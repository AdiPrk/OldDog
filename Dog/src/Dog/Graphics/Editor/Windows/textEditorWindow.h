#pragma once

#ifndef DOG_SHIP

class TextEditor;

namespace Dog {

	class TextEditorWrapper {
	public:
		TextEditorWrapper();
		~TextEditorWrapper();

        struct MyDocument
        {
            std::string Name;       // Document title
            bool        Open;       // Set when open (we keep an array of all available documents to simplify demo code!)
            bool        OpenPrev;   // Copy of Open from last update.
            bool        Dirty;      // Set when the document has been modified
            bool        WantClose;  // Set when the document
            std::string filePath;   // Full (relative) path to file

            // Text Editor:
            TextEditor Editor;

            MyDocument(const std::string& name = "New Document", bool open = true, const std::string& path = "");

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

        void NotifyOfDocumentsClosedElsewhere();

        ExampleAppDocuments app;
        
        void CreateNewDocument(MyDocument& doc);
	};

	void UpdateTextEditorWindow(TextEditorWrapper& textEditorWrapper);

}

#endif