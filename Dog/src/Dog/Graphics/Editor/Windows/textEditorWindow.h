#pragma once

class TextEditor;

namespace Dog {

	class TextEditorWrapper {
	public:
		TextEditorWrapper();
		~TextEditorWrapper();

        struct MyDocument
        {
            const char* Name;       // Document title
            bool        Open;       // Set when open (we keep an array of all available documents to simplify demo code!)
            bool        OpenPrev;   // Copy of Open from last update.
            bool        Dirty;      // Set when the document has been modified
            bool        WantClose;  // Set when the document
            std::string filePath;   // Full (relative) path to file

            // Text Editor:
            TextEditor Editor;

            MyDocument(const char* name = "New Document", bool open = true, const std::string& path = "");

            void DoOpen();
            void DoQueueClose();
            void DoForceClose();
            void DoSave();

            // Display placeholder contents for the Document
            static void DisplayContents(MyDocument* doc, TextEditor& editorTemp);

            // Display context menu for the Document
            static void DisplayContextMenu(MyDocument* doc);
        };

        struct ExampleAppDocuments
        {
            std::vector<TextEditorWrapper::MyDocument> Documents;
            ExampleAppDocuments();
        };

        static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments& app);
	};

	void UpdateTextEditorWindow(TextEditorWrapper& textEditorWrapper);

}
