#include "pch.h"
#include "scenarios.h"

static void PrintMenu()
{
    wprintf(L"\n");
    wprintf(L"========================================\n");
    wprintf(L"  Windows.UI.Shell.Tasks Test App\n");
    wprintf(L"========================================\n");
    wprintf(L"  1.  Check API Support\n");
    wprintf(L"  2.  Create Basic Task\n");
    wprintf(L"  3.  Create Task with Steps\n");
    wprintf(L"  4.  Create Task with Preview Thumbnail\n");
    wprintf(L"  5.  Create Task with Text Summary\n");
    wprintf(L"  6.  Create Task with Generated Assets\n");
    wprintf(L"  7.  Update Task State\n");
    wprintf(L"  8.  Update Task Titles\n");
    wprintf(L"  9.  Update Task Deep Link\n");
    wprintf(L"  10. Update Task Content\n");
    wprintf(L"  11. Add Interactive Elements\n");
    wprintf(L"  12. Enumerate All Tasks\n");
    wprintf(L"  13. Inspect Task Details\n");
    wprintf(L"  14. Remove Current Task\n");
    wprintf(L"  15. Remove All Tasks\n");
    wprintf(L"  16. Create Task with Actual Icon\n");
    wprintf(L"  0.  Exit\n");
    wprintf(L"========================================\n");
    wprintf(L"> ");
}

int wmain()
{
    winrt::init_apartment();

    TaskContext ctx{};

    wprintf(L"Shell Tasks API Test Application\n");
    wprintf(L"Windows.UI.Shell.Tasks namespace\n\n");

    for (;;)
    {
        PrintMenu();

        wchar_t buf[16] = {};
        if (!_getws_s(buf, _countof(buf)))
        {
            wprintf(L"Read error, exiting.\n");
            break;
        }

        int choice = _wtoi(buf);

        if (choice == 0)
        {
            wprintf(L"\nExiting...\n");
            return 0;
        }

        try
        {
            switch (choice)
            {
            case 1:  CheckApiSupport();                    break;
            case 2:  CreateBasicTask(ctx);                 break;
            case 3:  CreateTaskWithSteps(ctx);             break;
            case 4:  CreateTaskWithPreviewThumbnail(ctx);  break;
            case 5:  CreateTaskWithTextSummary(ctx);       break;
            case 6:  CreateTaskWithGeneratedAssets(ctx);    break;
            case 7:  UpdateTaskState(ctx);                 break;
            case 8:  UpdateTaskTitles(ctx);                break;
            case 9:  UpdateTaskDeepLink(ctx);              break;
            case 10: UpdateTaskContent(ctx);               break;
            case 11: AddInteractiveElements(ctx);          break;
            case 12: EnumerateTasks();                     break;
            case 13: InspectTaskDetails(ctx);              break;
            case 14: RemoveCurrentTask(ctx);               break;
            case 15: RemoveAllTasks();                     break;
            case 16: CreateTaskWithIcon(ctx);              break;

            default:
                wprintf(L"Invalid choice. Try again.\n");
                break;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            wprintf(L"\n[ERROR] HRESULT 0x%08X: %s\n", static_cast<uint32_t>(ex.code()), ex.message().c_str());
        }
    }

    return 0;
}
