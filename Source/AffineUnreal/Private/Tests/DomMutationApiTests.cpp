#if WITH_DEV_AUTOMATION_TESTS

#pragma push_macro("check")
#undef check
#include "affineui.h"
#pragma pop_macro("check")
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAffineUiDomMutationHandleTest,
	"AffineUI.EmbeddedDom.StableHandlesAcrossMovesAndInvalidation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAffineUiDomMutationHandleTest::RunTest(const FString& Parameters)
{
	(void)Parameters;
	affineui_ui* Ui = affineui_ui_create();
	if (!TestNotNull(TEXT("AffineUI instance is created"), Ui)) return false;
	affineui_ui_set_html(Ui, "<html><body><main id=\"app\"><section id=\"left\"></section><section id=\"right\"></section></main></body></html>");
	const affineui_dom_handle Root = affineui_ui_mount_root(Ui, "app");
	const affineui_dom_handle Left = affineui_ui_mount_root(Ui, "left");
	const affineui_dom_handle Right = affineui_ui_mount_root(Ui, "right");
	const affineui_dom_handle Counter = affineui_ui_create_node(Ui, AFFINEUI_DOM_ELEMENT, "span");
	const affineui_dom_handle Text = affineui_ui_create_node(Ui, AFFINEUI_DOM_TEXT, "0");
	TestTrue(TEXT("Mount and detached nodes receive generation handles"), Root.generation && Left.generation && Right.generation && Counter.generation && Text.generation);
	affineui_ui_begin_node_mutations(Ui);
	TestTrue(TEXT("Counter is inserted into its first parent"), affineui_ui_insert_node(Ui, Left, Counter, {}) != 0);
	TestTrue(TEXT("Text is inserted under the counter"), affineui_ui_insert_node(Ui, Counter, Text, {}) != 0);
	TestTrue(TEXT("Moving the counter preserves its handle"), affineui_ui_insert_node(Ui, Right, Counter, {}) != 0);
	TestTrue(TEXT("Moving the counter preserves its text child handle"), affineui_ui_node_valid(Ui, Text) != 0);
	TestTrue(TEXT("Text updates resolve by stable handle"), affineui_ui_node_set_text(Ui, Text, "1") != 0);
	affineui_ui_end_node_mutations(Ui);
	TestTrue(TEXT("Counter handle remains valid after the move"), affineui_ui_node_valid(Ui, Counter) != 0);
	TestTrue(TEXT("Removing a subtree succeeds"), affineui_ui_remove_node(Ui, Counter) != 0);
	TestFalse(TEXT("Removed element handle is rejected"), affineui_ui_node_valid(Ui, Counter) != 0);
	TestFalse(TEXT("Removed descendant handle is rejected"), affineui_ui_node_valid(Ui, Text) != 0);
	TestFalse(TEXT("Mutation through a removed handle is rejected"), affineui_ui_node_set_text(Ui, Text, "stale") != 0);
	affineui_ui_destroy(Ui);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAffineUiDomMutationStylesheetTest,
	"AffineUI.EmbeddedDom.StylesheetOnIncrementalNodes",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAffineUiDomMutationStylesheetTest::RunTest(const FString& Parameters)
{
	(void)Parameters;
	// The exported Vue stylesheet uses an unquoted, multi-word family name.
	affineui::Ui Ui;
	Ui.css(
		".native-counter{color:#eef0ff;background:#171b29;border:1px solid #39415b;"
		"border-radius:12px;padding:20px 24px;font-family:DM Sans,sans-serif}"
		".native-counter h1{color:#c9c2ff;margin:0 0 12px;font-size:18px;font-weight:600}"
		".native-counter__label{color:#fff;margin:0;font-size:40px;font-weight:700;line-height:1}"
		"\n/*$vite$:1*/");
	Ui.html("<html><body><main id=\"app\"></main></body></html>");

	Ui.begin_node_mutations();
	const auto Root = Ui.mount_root("app");
	const auto Panel = Ui.create_node(affineui::DomNodeKind::Element, "section");
	const auto Heading = Ui.create_node(affineui::DomNodeKind::Element, "h1");
	const auto HeadingText = Ui.create_node(affineui::DomNodeKind::Text, "Vue runtime-core");
	TestTrue(TEXT("Heading text is inserted"), Ui.insert_node(Heading, HeadingText));
	TestTrue(TEXT("Detached heading receives a lookup ID"), Ui.set_node_attribute(Heading, "id", "heading"));
	TestTrue(TEXT("Heading is inserted into the detached panel"), Ui.insert_node(Panel, Heading));

	const auto Label = Ui.create_node(affineui::DomNodeKind::Element, "p");
	const auto Count = Ui.create_node(affineui::DomNodeKind::Text, "0");
	TestTrue(TEXT("Count text is inserted"), Ui.insert_node(Label, Count));
	TestTrue(TEXT("Detached label receives its class"), Ui.set_node_attribute(Label, "class", "native-counter__label"));
	TestTrue(TEXT("Label is inserted into the detached panel"), Ui.insert_node(Panel, Label));
	TestTrue(TEXT("Detached panel receives its class"), Ui.set_node_attribute(Panel, "class", "native-counter"));
	TestTrue(TEXT("Panel subtree is mounted"), Ui.insert_node(Root, Panel));
	Ui.end_node_mutations();

	affineui::Document& Document = Ui.document();
	Document.layout(480, 240);
	const affineui::Rect PanelRect = Document.find_element_rect("[class=native-counter]");
	const affineui::Rect HeadingRect = Document.find_element_rect("#heading");
	const affineui::Rect LabelRect = Document.find_element_rect("[class=native-counter__label]");
	TestEqual(TEXT("Panel class applies horizontal padding and border"), HeadingRect.x - PanelRect.x, 25);
	TestEqual(TEXT("Heading descendant selector applies bottom margin"), LabelRect.y - (HeadingRect.y + HeadingRect.h), 12);
	TestEqual(TEXT("Label class applies font size and line height"), LabelRect.h, 40);
	return true;
}

#endif
