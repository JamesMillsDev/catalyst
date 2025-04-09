//#include "Application.h"
//
//using Catalyst::Application;
//
//int main()
//{
//   return Application::Run();
//}

#include <GL/glew.h>    // Include GLEW for OpenGL extension loading
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <thread>
#include <functional>
#include <atomic>
#include <chrono>
#include <mutex>
#include <set>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL headers.

namespace fs = std::filesystem;

class DirectoryWatcher {
public:
	using Callback = std::function<void(const std::string&)>;

	DirectoryWatcher(const std::string& path, unsigned int intervalMs = 2000)
		: m_watchPath(path), m_intervalMs(intervalMs), m_running(false) {
		scanDirectory(); // Initial scan
	}

	~DirectoryWatcher() {
		stop();
	}

	void onFileAdded(Callback callback) { m_onFileAdded = std::move(callback); }
	void onFileDeleted(Callback callback) { m_onFileDeleted = std::move(callback); }
	void onDirectoryAdded(Callback callback) { m_onDirectoryAdded = std::move(callback); }
	void onDirectoryDeleted(Callback callback) { m_onDirectoryDeleted = std::move(callback); }

	void start() {
		if (m_running.load()) return; // Already running

		m_running.store(true);
		m_watcherThread = std::thread([this]() {
			while (m_running.load()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(m_intervalMs));
				checkForChanges();
			}
			});
	}

	void stop() {
		m_running.store(false);
		if (m_watcherThread.joinable()) {
			m_watcherThread.join();
		}
	}

	void getFilesAndFolders(std::unordered_map<std::string, std::vector<std::string>>& structure) {
		std::lock_guard<std::mutex> lock(m_structureMutex);
		structure = m_directoryStructure;
	}

public:
	std::string m_watchPath;
	unsigned int m_intervalMs;
	std::atomic<bool> m_running;
	std::thread m_watcherThread;
	std::mutex m_structureMutex;

	// Store directory structure with files
	std::unordered_map<std::string, std::vector<std::string>> m_directoryStructure;

	Callback m_onFileAdded;
	Callback m_onFileDeleted;
	Callback m_onDirectoryAdded;
	Callback m_onDirectoryDeleted;

	void scanDirectory() {
		m_directoryStructure.clear();

		for (const auto& entry : fs::recursive_directory_iterator(m_watchPath)) {
			if (entry.is_directory()) {
				m_directoryStructure[entry.path().string()] = {};
			}
			else if (entry.is_regular_file()) {
				// Find the parent directory for each file
				auto parentPath = entry.path().parent_path().string();
				m_directoryStructure[parentPath].push_back(entry.path().string());
			}
		}
	}

	void checkForChanges() {
		std::unordered_map<std::string, std::vector<std::string>> currentStructure;

		for (const auto& entry : fs::recursive_directory_iterator(m_watchPath)) {
			if (entry.is_directory()) {
				currentStructure[entry.path().string()] = {};
			}
			else if (entry.is_regular_file()) {
				auto parentPath = entry.path().parent_path().string();
				currentStructure[parentPath].push_back(entry.path().string());
			}
		}

		// Check for added directories
		for (const auto& [dir, _] : currentStructure) {
			if (m_directoryStructure.find(dir) == m_directoryStructure.end()) {
				if (m_onDirectoryAdded) {
					m_onDirectoryAdded(dir);
				}
			}
		}

		// Check for added files
		for (const auto& [dir, files] : currentStructure) {
			for (const auto& file : files) {
				if (std::find(m_directoryStructure[dir].begin(), m_directoryStructure[dir].end(), file) == m_directoryStructure[dir].end()) {
					if (m_onFileAdded) {
						m_onFileAdded(file);
					}
				}
			}
		}

		// Check for deleted files and directories
		for (const auto& [dir, _] : m_directoryStructure) {
			if (currentStructure.find(dir) == currentStructure.end()) {
				if (m_onDirectoryDeleted) {
					m_onDirectoryDeleted(dir);
				}
			}
		}
		for (auto& [dir, files] : m_directoryStructure) {
			for (const auto& file : files) {
				if (std::find(currentStructure[dir].begin(), currentStructure[dir].end(), file) == currentStructure[dir].end()) {
					if (m_onFileDeleted) {
						m_onFileDeleted(file);
					}
				}
			}
		}

		// Update the directory structure
		std::lock_guard<std::mutex> lock(m_structureMutex);
		m_directoryStructure = std::move(currentStructure);
	}
};

void initImGui(GLFWwindow*& window, bool& done) {
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed!" << std::endl;
		done = true;
		return;
	}

	window = glfwCreateWindow(1280, 720, "Directory and File Watcher", NULL, NULL);
	if (window == NULL) {
		std::cerr << "GLFW window creation failed!" << std::endl;
		done = true;
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	GLenum glewInitRes = glewInit();
	if (glewInitRes != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewInitRes) << std::endl;
		done = true;
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

// Function to get files in a specific directory (without extensions)
std::vector<std::string> getFilesInDirectory(const std::string& directory) {
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			// Add file without extension
			files.push_back(entry.path().string());
		}
	}
	return files;
}

// Recursive function to display directories and files correctly
void displayDirectoryTree(const std::string& directory) {
	std::string folderName = fs::path(directory).filename().string(); // Get just the folder name

	if (ImGui::TreeNode(folderName.c_str())) {
		// Show files inside the folder
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				std::string fileName = entry.path().stem().string(); // Get file name without extension
				ImGui::BulletText(fileName.c_str());
			}
		}

		// Recursively display subdirectories
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_directory()) {
				displayDirectoryTree(entry.path().string());
			}
		}

		ImGui::TreePop();
	}
}

void renderUI(DirectoryWatcher& watcher) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Directory and File Watcher");

	std::string rootPath = watcher.m_watchPath; // Get the base directory being watched
	std::vector<std::string> rootFolders;

	// Find only the immediate children (root-level directories)
	for (const auto& entry : fs::directory_iterator(rootPath)) {
		if (entry.is_directory()) {
			rootFolders.push_back(entry.path().string());
		}
	}

	// Display only the root-level folders
	for (const auto& rootFolder : rootFolders) {
		displayDirectoryTree(rootFolder);
	}

	ImGui::End();

	ImGui::Render();
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



int main() {
	GLFWwindow* window = nullptr;
	bool done = false;
	initImGui(window, done);
	if (done) return -1;

	DirectoryWatcher watcher(R"(G:\Development\C++\TestProject)");
	watcher.onFileAdded([&watcher](const std::string& path) {
		std::string message = "[+] File Added: " + path;
		std::lock_guard<std::mutex> lock(watcher.m_structureMutex);
		auto parentPath = fs::path(path).parent_path().string();
		watcher.m_directoryStructure[parentPath].push_back(path);
		});
	watcher.onFileDeleted([&watcher](const std::string& path) {
		std::string message = "[-] File Deleted: " + path;
		std::lock_guard<std::mutex> lock(watcher.m_structureMutex);
		auto parentPath = fs::path(path).parent_path().string();
		auto& files = watcher.m_directoryStructure[parentPath];
		files.erase(std::remove(files.begin(), files.end(), path), files.end());
		});
	watcher.onDirectoryAdded([&watcher](const std::string& path) {
		std::string message = "[+] Directory Added: " + path;
		std::lock_guard<std::mutex> lock(watcher.m_structureMutex);
		watcher.m_directoryStructure[path] = {};
		});
	watcher.onDirectoryDeleted([&watcher](const std::string& path) {
		std::string message = "[-] Directory Deleted: " + path;
		std::lock_guard<std::mutex> lock(watcher.m_structureMutex);
		watcher.m_directoryStructure.erase(path);
		});
	watcher.start();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderUI(watcher);
		glfwSwapBuffers(window);
	}

	watcher.stop();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

