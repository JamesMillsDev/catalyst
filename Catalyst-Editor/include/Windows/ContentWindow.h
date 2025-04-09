#pragma once

#include "EditorWindow.h"

#include <filesystem>

#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

using fs::file_time_type;

using std::atomic;
using std::function;
using std::mutex;
using std::pair;
using std::thread;
using std::unordered_map;
using std::vector;

namespace Catalyst
{
	class Texture;

	enum class EContentType;

	class DirectoryObserver
	{
	private:
		typedef function<void(const string&, const string&)> Callback;

		friend class ContentWindow;

	private:
		string m_path;
		uint32_t m_intervalMs;

		atomic<bool> m_running;
		thread m_observerThread;
		mutex m_eventMutex;

		unordered_map<string, file_time_type> m_fileTimestamps;
		vector<string> m_eventLog;

		unordered_map<string, unordered_map<string, EContentType>> m_directoryTree;

		Callback m_onFileAdded;
		Callback m_onFileModified;
		Callback m_onFileDeleted;

	private:
		DirectoryObserver(const string& path, unsigned int intervalMs = 2000);
		~DirectoryObserver();

	private:
		void OnFileAdded(Callback callback);
		void OnFileModified(Callback callback);
		void OnFileDeleted(Callback callback);

		void GetEvents(vector<string>& events);

		void Start();
		void Stop();

		void ScanDirectory();
		void CheckForChanges();

		void AddFile(const string& path);
		EContentType GetContentTypeFromExtension(const string& ext);

	};

	class ContentWindow final : public EditorWindow
	{
	public:
		ContentWindow();
		~ContentWindow() override;

	protected:
		void OnGui(EditorApplication* app) override;

	private:
		DirectoryObserver* m_observer;

		pair<string, string> m_selectedFolder;

		unordered_map<EContentType, Texture*> m_icons;

	private:
		void InitialiseIcons();
		void InitialiseIcon(EContentType type, const string& iconName);
		ImTextureID GetIconFor(EContentType type);

		void DisplayDirectoryTree(const string& rootDir);
		void DisplayFolderContents();

		void DisplayDirectoryNode(const string& directory, const string& rootDir, ImGuiTreeNodeFlags& flags, bool isSelected, bool& isOpen, float _fullWidth, ImVec2 selectableSize, bool isDirectory);
		ImGuiTreeNodeFlags GetBaseFlags(bool isDirectory);

	};
}

