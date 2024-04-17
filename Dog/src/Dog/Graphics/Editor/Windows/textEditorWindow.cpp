#include <PCH/dogpch.h>
#include "textEditorWindow.h"

namespace Dog {

    std::string readFileToString(const std::string& filename) {
        std::ifstream file(filename);  // Open the file for input
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();  // Read the file buffer into the stringstream

        return buffer.str();  // Convert the stringstream to std::string and return
    }

    void writeStringToFile(const std::string& filename, const std::string& data) {
        std::ofstream file(filename);  // Open the file for output
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        file << data;  // Write the data to the file

        if (!file) {  // Check for write errors
            throw std::runtime_error("Error writing to file: " + filename);
        }
    }

    TextEditorWrapper::MyDocument::MyDocument(const std::string& name, bool open, const std::string& path)
        : Name(name), Open(open), OpenPrev(open), Dirty(false), WantClose(false), filePath(path), Editor()
    {
        Editor.SetShowWhitespaces(false);

        if (path != "")
        {
			Editor.SetText(readFileToString(path));
		}
    }

    TextEditorWrapper::TextEditorWrapper()
        : app()
    {
    }

	TextEditorWrapper::~TextEditorWrapper()
	{
	}

    void TextEditorWrapper::NotifyOfDocumentsClosedElsewhere()
    {
        for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
        {
            TextEditorWrapper::MyDocument* doc = &app.Documents[doc_n];
            if (!doc->Open && doc->OpenPrev)
                ImGui::SetTabItemClosed(doc->Name.c_str());
            doc->OpenPrev = doc->Open;
        }
    }

    void TextEditorWrapper::CreateNewDocument(MyDocument& doc)
    {
        for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
        {
			TextEditorWrapper::MyDocument* existingDoc = &app.Documents[doc_n];
            if (existingDoc->filePath == doc.filePath)
            {
                if (!existingDoc->Open)
				    existingDoc->DoOpen();
                else {
                    // switch to that doc.
                    ImGui::SetWindowFocus(existingDoc->Name.c_str());
                }
				return;
			}
		}

        app.Documents.push_back(doc);
    }

    TextEditorWrapper::ExampleAppDocuments::ExampleAppDocuments()
    {
        // Can create documents that always spawn on startup here
    }

    void ShaderDragDrop(TextEditorWrapper& textEditorWrapper) {
        if (ImGui::BeginDragDropTarget()) {
            // Accept data that has the identifier "DRAG_INT"
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Shader")) {
                // get string from payload data and data size
                std::string path = std::string((char*)payload->Data, payload->DataSize - 1); // -1 to remove null terminator
                printf("Path from drag drop: %s\n", path.c_str());

                std::string fullPath = Resources::ShadersPath + path;
                std::string vertPath = fullPath + ".vert";
                std::string fragPath = fullPath + ".frag";
                std::string vertName = path + ".vert";
                std::string fragName = path + ".frag";

                TextEditorWrapper::MyDocument vertDoc(vertName, true, vertPath);
                TextEditorWrapper::MyDocument fragDoc(fragName, true, fragPath);

                textEditorWrapper.CreateNewDocument(vertDoc);
                textEditorWrapper.CreateNewDocument(fragDoc);

                //shaderPath = path;
            }

            ImGui::EndDragDropTarget();
        }
    }

	void UpdateTextEditorWindow(TextEditorWrapper& textEditorWrapper) {
        auto& app = textEditorWrapper.app;

        // Options
        static bool opt_reorderable = true;
        static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

        static std::unique_ptr<bool> p_open = std::make_unique<bool>(true);
        if (!(*p_open)) return; // Don't even show this window.

        bool window_contents_visible = ImGui::Begin("Text Editor", p_open.get(), ImGuiWindowFlags_MenuBar);
        if (!window_contents_visible)
        {
            ImGui::End();
            return;
        }

        // Menu
        bool redock_all = false;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                int open_count = 0;
                for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
                    open_count += app.Documents[doc_n].Open ? 1 : 0;

                if (ImGui::BeginMenu("Open Recent", open_count < app.Documents.size()))
                {
                    for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
                    {
                        TextEditorWrapper::MyDocument* doc = &app.Documents[doc_n];
                        if (!doc->Open)
                            if (ImGui::MenuItem(doc->Name.c_str()))
                                doc->DoOpen();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Redock All Documents", NULL, false, open_count > 0))
                    redock_all = true;
                if (ImGui::MenuItem("Close All Documents", NULL, false, open_count > 0)) 
                {
                    for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++) {
                        app.Documents[doc_n].DoQueueClose();
                    }
                }
                if (ImGui::MenuItem("Exit", "Ctrl+F4") && p_open)
                    *p_open = false;

                ImGui::EndMenu(); // File
            }
            ImGui::EndMenuBar(); // Menu
        }

        ImGui::Separator();

        // About the ImGuiWindowFlags_UnsavedDocument / ImGuiTabItemFlags_UnsavedDocument flags.
        // They have multiple effects:
        // - Display a dot next to the title.
        // - Tab is selected when clicking the X close button.
        // - Closure is not assumed (will wait for user to stop submitting the tab).
        //   Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
        //   We need to assume closure by default otherwise waiting for "lack of submission" on the next frame would leave an empty
        //   hole for one-frame, both in the tab-bar and in tab-contents when closing a tab/window.
        //   The rarely used SetTabItemClosed() function is a way to notify of programmatic closure to avoid the one-frame hole.

        // Tabs
        
        textEditorWrapper.NotifyOfDocumentsClosedElsewhere();

        int numDocumentsOpen = 0;
        for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
			numDocumentsOpen += app.Documents[doc_n].Open ? 1 : 0;

        if (numDocumentsOpen == 0) {
            ImGui::TextWrapped("No documents open.");
            ImGui::TextWrapped("Drag and drop a shader file from the 'Resources' tab to open it!");
        }

        // Create a DockSpace node where any window can be docked
        ImGuiID dockspace_id = ImGui::GetID("TextEditorDockspace");
        ImGui::DockSpace(dockspace_id);

        ShaderDragDrop(textEditorWrapper);
        // Create Windows
        for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
        {
            TextEditorWrapper::MyDocument* doc = &app.Documents[doc_n];
            if (!doc->Open)
                continue;

            ImGui::SetNextWindowDockID(dockspace_id, redock_all ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
            ImGuiWindowFlags window_flags = (doc->Dirty ? ImGuiWindowFlags_UnsavedDocument : 0);
            window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
            bool visible = ImGui::Begin(doc->Name.c_str(), &doc->Open, window_flags);

            // Cancel attempt to close when unsaved add to save queue so we can display a popup.
            if (!doc->Open && doc->Dirty)
            {
                doc->Open = true;
                doc->DoQueueClose();
            }

            TextEditorWrapper::MyDocument::DisplayContextMenu(doc);
            if (visible) {
                TextEditorWrapper::MyDocument::DisplayContents(doc, doc->Editor);
                // if ctrl shift s, save
                auto& io = ImGui::GetIO();
                if (io.KeyCtrl && io.KeysDown[ImGuiKey_S]) {
                    if (doc->Editor.GetFocused()) {
                        if (doc->Dirty) {
                            doc->DoSave();
                        }
                    }
                }

                static bool wTriggered = false;
                if (!wTriggered && io.KeyCtrl && io.KeysDown[ImGuiKey_W]) {
					doc->DoQueueClose();
                    wTriggered = true;
				}
                else if (!io.KeysDown[ImGuiKey_W]) {
					wTriggered = false;
				}
            }

            ShaderDragDrop(textEditorWrapper);

            ImGui::End(); // Document
        }

        // Update closing queue
        static ImVector<TextEditorWrapper::MyDocument*> close_queue;
        if (close_queue.empty())
        {
            // Close queue is locked once we started a popup
            for (int doc_n = 0; doc_n < app.Documents.size(); doc_n++)
            {
                TextEditorWrapper::MyDocument* doc = &app.Documents[doc_n];
                if (doc->WantClose)
                {
                    doc->WantClose = false;
                    close_queue.push_back(doc);
                }
            }
        }

        // Display closing confirmation UI
        if (!close_queue.empty())
        {
            int close_queue_unsaved_documents = 0;
            for (int n = 0; n < close_queue.Size; n++)
                if (close_queue[n]->Dirty)
                    close_queue_unsaved_documents++;

            if (close_queue_unsaved_documents == 0)
            {
                // Close documents when all are unsaved
                for (int n = 0; n < close_queue.Size; n++)
                    close_queue[n]->DoForceClose();
                close_queue.clear();
            }
            else
            {
                if (!ImGui::IsPopupOpen("Save?"))
                    ImGui::OpenPopup("Save?");
                if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Save change to the following items?");
                    float item_height = ImGui::GetTextLineHeightWithSpacing();
                    if (ImGui::BeginChildFrame(ImGui::GetID("frame"), ImVec2(-FLT_MIN, 6.25f * item_height)))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                            if (close_queue[n]->Dirty)
                                ImGui::Text("%s", close_queue[n]->Name.c_str());
                    }
                    ImGui::EndChildFrame();

                    ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
                    if (ImGui::Button("Yes", button_size))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                        {
                            if (close_queue[n]->Dirty)
                                close_queue[n]->DoSave();
                            close_queue[n]->DoForceClose();
                        }
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No", button_size))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                            close_queue[n]->DoForceClose();
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", button_size))
                    {
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }

        ImGui::End(); 
	}

    void TextEditorWrapper::MyDocument::DoOpen() {
        Open = true; 
        // printf("Open %s\n", Name);
        // reset text to path.
        Editor.SetText(readFileToString(filePath));
    }

    void TextEditorWrapper::MyDocument::DoQueueClose() { 
        WantClose = true;
        // printf("QueueClose %s\n", Name);
    }

    void TextEditorWrapper::MyDocument::DoForceClose() { 
        Open = false;
        Dirty = false;
        // printf("ForceClose %s\n", Name);

    }

    void TextEditorWrapper::MyDocument::DoSave() {
        Dirty = false;
        // printf("Save %s\n", Name.c_str());

        writeStringToFile(filePath, Editor.GetText());
    }

    // Display placeholder contents for the Document

    void TextEditorWrapper::MyDocument::DisplayContents(MyDocument* doc, TextEditor& editorTemp)
    {
        editorTemp.Render(doc->Name.c_str());

        if (editorTemp.IsTextChanged()) {
            doc->Dirty = true;
        }
    }

    // Display context menu for the Document

    void TextEditorWrapper::MyDocument::DisplayContextMenu(MyDocument* doc)
    {
        if (!ImGui::BeginPopupContextItem())
            return;

        char buf[256];
        // use safe version of sprintf; sprintf_s
        sprintf_s(buf, "Save %s", doc->Name.c_str());
        if (ImGui::MenuItem(buf, "CTRL+S", false, doc->Open))
            doc->DoSave();
        if (ImGui::MenuItem("Close", "CTRL+W", false, doc->Open))
            doc->DoQueueClose();
        ImGui::EndPopup();
    }

}
