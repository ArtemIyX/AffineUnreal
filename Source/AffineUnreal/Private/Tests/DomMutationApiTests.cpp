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

#endif
