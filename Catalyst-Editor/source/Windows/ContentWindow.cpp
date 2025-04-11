#include "Windows/ContentWindow.h"

#include "EditorApplication.h"
#include "EditorGUI.h"

#include <Rendering/Texture.h>

#include "Assets/Asset.h"

namespace this_thread = std::this_thread;

using std::lock_guard;

using std::chrono::milliseconds;

using fs::directory_iterator;
using fs::path;
using fs::recursive_directory_iterator;

namespace Catalyst
{
	DirectoryObserver::DirectoryObserver(const string& path, unsigned int intervalMs)
		: m_path{ path }, m_intervalMs{ intervalMs }, m_running{ false }
	{
		ScanDirectory();
	}

	DirectoryObserver::~DirectoryObserver()
	{
		Stop();
	}

	void DirectoryObserver::OnFileAdded(Callback callback)
	{
		m_onFileAdded = callback;
	}

	void DirectoryObserver::OnFileModified(Callback callback)
	{
		m_onFileModified = callback;
	}

	void DirectoryObserver::OnFileDeleted(Callback callback)
	{
		m_onFileDeleted = callback;
	}

	void DirectoryObserver::GetEvents(vector<string>& events)
	{
		lock_guard<mutex> lock(m_eventMutex);
		events = m_eventLog;
	}

	void DirectoryObserver::Start()
	{
		if (m_running.load())
		{
			return;
		}

		m_running.store(true);

		m_observerThread = thread([this]()
			{
				while (m_running.load())
				{
					this_thread::sleep_for(milliseconds(m_intervalMs));
					CheckForChanges();
				}
			}
		);
	}

	void DirectoryObserver::Stop()
	{
		m_running.store(false);

		if (m_observerThread.joinable())
		{
			m_observerThread.join();
		}

		for (auto& [id, tree] : m_directoryTree)
		{
			tree.clear();
		}

		m_directoryTree.clear();
	}

	void DirectoryObserver::ScanDirectory()
	{
		for (const auto& entry : recursive_directory_iterator(m_path))
		{
			string parentDir = entry.path().parent_path().string();
			string fileName = entry.path().filename().stem().string(); // Remove extension

			if (fs::is_directory(entry))
			{
				m_directoryTree[parentDir][fileName] = EContentType::Directory;
			}
			else if (fs::is_regular_file(entry))
			{
				m_directoryTree[parentDir][fileName] = GetContentTypeFromExtension(entry.path().extension().string()); // Add file (false means it's a file)
			}
		}
	}

	void DirectoryObserver::CheckForChanges()
	{
		unordered_map<string, file_time_type> newTimestamps;
		vector<string> deletedFiles;

		for (const auto& entry : recursive_directory_iterator(m_path))
		{
			const string& pathStr = entry.path().string();
			newTimestamps[pathStr] = fs::last_write_time(entry);

			if (m_fileTimestamps.find(pathStr) == m_fileTimestamps.end())
			{
				if (m_onFileAdded)
				{
					m_onFileAdded(pathStr, "added");
				}
			}
			else if (m_fileTimestamps[pathStr] != fs::last_write_time(entry))
			{
				if (m_onFileModified)
				{
					m_onFileModified(pathStr, "modified");
				}
			}
		}

		for (const auto& [oldPath, _] : m_fileTimestamps)
		{
			if (newTimestamps.find(oldPath) == newTimestamps.end())
			{
				deletedFiles.emplace_back(oldPath);
			}
		}

		for (const auto& deletedFile : deletedFiles)
		{
			if (m_onFileDeleted)
			{
				m_onFileDeleted(deletedFile, "deleted");
			}
		}

		m_fileTimestamps = std::move(newTimestamps);
	}

	void DirectoryObserver::AddFile(const string& path)
	{
		fs::path p = fs::path(path);

		std::string folderName = p.filename().stem().string(); // Remove extension
		if (fs::is_directory(path))
		{
			// Add directory to the tree
			m_directoryTree[p.parent_path().string()][folderName] = EContentType::Directory;
		}
		else
		{
			string extension = p.extension().string();

			// Add file to the tree
			m_directoryTree[p.parent_path().string()][folderName] = GetContentTypeFromExtension(extension);
		}
	}

	EContentType DirectoryObserver::GetContentTypeFromExtension(const string& ext)
	{
		string extension = ext;
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".shader")
		{
			return EContentType::Shader;
		}

		if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".tga")
		{
			return EContentType::Texture;
		}

		if (extension == ".fbx" || extension == ".obj")
		{
			return EContentType::Mesh;
		}

		return EContentType::Invalid;
	}

	ContentWindow::ContentWindow()
		: EditorWindow{ "Content Browser" }, m_observer{ nullptr }
	{
		InitialiseIcons();
	}

	ContentWindow::~ContentWindow()
	{
		for (const auto& [_, texture] : m_icons)
		{
			delete texture;
		}

		m_icons.clear();

		delete m_observer;
	}

	void ContentWindow::OnGui(EditorApplication* app)
	{
		if (m_observer == nullptr)
		{
			m_observer = new DirectoryObserver(app->GetProjectContentDir());
			m_observer->OnFileAdded([&, this](const string& path, const string& event)
				{
					m_observer->AddFile(path);
				});

			m_observer->OnFileDeleted([&](const string& path, const string& event)
				{
					string folderName = fs::path(path).filename().stem().string(); // Remove extension
					if (fs::is_directory(path))
					{
						// Remove directory from the tree
						m_observer->m_directoryTree[fs::path(path).parent_path().string()].erase(folderName);
					}
					else
					{
						// Remove file from the tree
						m_observer->m_directoryTree[fs::path(path).parent_path().string()].erase(folderName);
					}
				});

			m_observer->Start();
		}

		ImGui::Columns(2, "content_browser_columns");
		ImGui::Separator();

		// Get the available space in the window
		ImVec2 windowSize = ImGui::GetWindowSize();
		float windowWidth = windowSize.x;

		// Account for window padding (if any)
		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		float availableWidth = windowWidth - padding.x * 2;  // subtract padding

		// Set the width of the first column (fixed size)
		float firstColumnWidth = 350.0f;  // Fixed width for the first column
		// Start columns
		ImGui::Columns(2, "my_columns", true); // The "true" enables column resizing

		// Set the width of the first column ONLY on the first frame
		static bool initialized = false;
		if (!initialized)
		{
			ImGui::SetColumnWidth(0, firstColumnWidth); // Set initial width

			initialized = true;
		}

		ImGui::BeginChild("ScrollableTree", ImVec2(0, 0), true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
		{
			string folderName = fs::path(app->GetProjectContentDir()).filename().stem().string();
			bool open = ImGui::TreeNode(folderName.c_str());
			if (open)
			{
				DisplayDirectoryTree(app->GetProjectContentDir());
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("ScrollableFolder", ImVec2(0, 0), true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
		{
			DisplayFolderContents();
		}
		ImGui::EndChild();

		ImGui::Columns(1);
	}

	void ContentWindow::InitialiseIcons()
	{
		InitialiseIcon(EContentType::Directory, "folder");
		InitialiseIcon(EContentType::Texture, "texture");
		InitialiseIcon(EContentType::Mesh, "mesh");
		InitialiseIcon(EContentType::Material, "material");
		//InitialiseIcon(EContentType::Sound, "sound");
		InitialiseIcon(EContentType::Shader, "shader");
		//InitialiseIcon(EContentType::Script, "script");
	}

	void ContentWindow::InitialiseIcon(EContentType type, const string& iconName)
	{
		m_icons[type] = new Texture(std::format("resources/icons/{}.png", iconName).c_str());
	}

	ImTextureID ContentWindow::GetIconFor(EContentType type)
	{
		if (m_icons.contains(type))
		{
			return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_icons[type]->GetHandle()));
		}

		return nullptr;
	}

	void ContentWindow::DisplayDirectoryTree(const string& rootDir)
	{
		if (m_observer->m_directoryTree.find(rootDir) != m_observer->m_directoryTree.end())
		{
			float fullWidth = ImGui::GetContentRegionAvail().x;

			for (auto& [folder, type] : m_observer->m_directoryTree[rootDir])
			{
				ImGui::PushID(&folder);

				bool isSelected = m_selectedFolder.first == rootDir && m_selectedFolder.second == folder;

				ImVec2 selectableSize = ImGui::CalcTextSize(folder.c_str());
				selectableSize.x += ImGui::GetStyle().FramePadding.x * 2;
				selectableSize.y += ImGui::GetStyle().FramePadding.y * 2;

				bool isDirectory = type == EContentType::Directory;
				ImGuiTreeNodeFlags flags = GetBaseFlags(isDirectory);

				bool nodeOpen = false;
				DisplayDirectoryNode(folder, rootDir, flags, isSelected, nodeOpen, fullWidth, selectableSize, isDirectory);

				if (nodeOpen)
				{
					DisplayDirectoryTree(rootDir + "\\" + folder);
					ImGui::TreePop();
				}

				ImGui::PopID();
			}
		}
	}

	void ContentWindow::DisplayFolderContents()
	{
		string rootDir = m_selectedFolder.first;
		string directory = m_selectedFolder.second;

		if (directory == "" && rootDir == "")
		{
			return;
		}

		string folder = rootDir + "\\" + directory;

		if (!fs::exists(folder))
		{
			path parentPath = rootDir;

			m_selectedFolder.first = parentPath.parent_path().string();
			m_selectedFolder.second = parentPath.stem().string();

			DisplayFolderContents();
			return;
		}

		ImGui::BeginGroup();
		{
			ImVec2 tileSize = ImVec2(80.f, 90.f);
			const float spacing = 5.0f;   // Space between tiles
			const float windowWidth = ImGui::GetContentRegionAvail().x;
			const int columns = (int)((windowWidth + spacing) / (tileSize.x + spacing));

			ImVec2 imageSize = ImVec2(64.f, 64.f);

			unordered_map<string, EContentType> folderContents = m_observer->m_directoryTree[folder];
			int i = 0;

			for (const auto& [name, type] : folderContents)
			{
				ImGui::PushID(i); // To prevent clashes in IDs

				if (i > 0 && i % columns == 0)
				{
					ImGui::NewLine(); // New row of tiles
				}

				if (type == EContentType::Directory)
				{
					if (EditorGUI::ImageButtonWithLabel(GetIconFor(type), name.c_str(), imageSize, tileSize, true))
					{
						// Only update selection on double-click
						m_selectedFolder = std::make_pair(folder, name);
					}
				}
				else
				{
					if (EditorGUI::ImageButtonWithLabel(GetIconFor(type), name.c_str(), imageSize, tileSize))
					{
						// We want this to be handled as a double click, so we ignore it
					}
				}

				ImGui::PopID();
				ImGui::SameLine();
				i++;
			}
		}
		ImGui::EndGroup();

		//// Get the contents of the selected folder
		//vector<string> files;
		//for (const auto& entry : fs::directory_iterator(folder)) 
		//{
		//	files.push_back(entry.path().filename().string()); // Only get the file/folder name without full path
		//}

		//// Determine the number of columns for the tile layout
		//const float tileSize = 120.0f; // Width and height of each tile
		//const float spacing = 10.0f;   // Space between tiles
		//const float windowWidth = ImGui::GetContentRegionAvail().x;
		//const int columns = (int)((windowWidth + spacing) / (tileSize + spacing));

		//// Create a grid layout
		//ImGui::BeginGroup();
		//for (size_t i = 0; i < files.size(); ++i) 
		//{
		//	if (i > 0 && i % columns == 0) 
		//	{
		//		ImGui::NewLine(); // New row of tiles
		//	}

		//	// Make each tile a button or image (you can replace it with actual images or icons)
		//	ImGui::PushID(i); // To prevent clashes in IDs

		//	bool isFolder = fs::is_directory(folder + "\\" + files[i]); // Check if it's a folder
		//	if (isFolder) 
		//	{
		//		if (ImGui::Button((files[i]).c_str(), ImVec2(tileSize, tileSize)))
		//		{
		//			// We want this to be handled as a double click, so we ignore it
		//		}

		//		// Check if the button was double-clicked
		//		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) 
		//		{
		//			// Only update selection on double-click
		//			m_selectedFolder = std::make_pair(folder, files[i]);
		//		}
		//	}
		//	else 
		//	{
		//		if (ImGui::Button((files[i]).c_str(), ImVec2(tileSize, tileSize)))
		//		{
		//			// Handle file click
		//			/*std::cout << "Clicked on file: " << files[i] << std::endl;*/
		//		}
		//	}

		//	ImGui::PopID();
		//	ImGui::SameLine();
		//}
		//ImGui::EndGroup();
	}

	void ContentWindow::DisplayDirectoryNode(const string& directory, const string& rootDir, ImGuiTreeNodeFlags& flags, bool isSelected, bool& isOpen, float fullWidth, ImVec2 selectableSize, bool isDirectory)
	{
		if (isSelected)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImGui::GetCursorScreenPos(),
				ImVec2(ImGui::GetCursorScreenPos().x + fullWidth, ImGui::GetCursorScreenPos().y + selectableSize.y),
				ImGui::GetColorU32(ImGuiCol_Header)
			);
		}

		if (isSelected)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		isOpen = ImGui::TreeNodeEx(directory.c_str(), flags, "%s", directory.c_str());

		if (ImGui::IsItemClicked() && isDirectory)
		{
			m_selectedFolder = std::make_pair(rootDir, directory);
		}
	}

	ImGuiTreeNodeFlags ContentWindow::GetBaseFlags(bool isDirectory)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (!isDirectory)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// Add the OpenOnArrow flag for handling expansion with the arrow click
		if (isDirectory)
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		}

		return flags;
	}
}
