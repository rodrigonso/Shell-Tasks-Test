#include "pch.h"
#include "scenarios.h"

static const wchar_t* StateToString(AppTaskState state)
{
    switch (state)
    {
    case AppTaskState::Running:        return L"Running";
    case AppTaskState::Completed:      return L"Completed";
    case AppTaskState::NeedsAttention: return L"NeedsAttention";
    case AppTaskState::Paused:         return L"Paused";
    case AppTaskState::Error:          return L"Error";
    default:                           return L"Unknown";
    }
}

static void PrintTaskInfo(AppTaskInfo const& task)
{
    wprintf(L"  ID:       %s\n", task.Id().c_str());
    wprintf(L"  Title:    %s\n", task.Title().c_str());
    wprintf(L"  Subtitle: %s\n", task.Subtitle().c_str());
    wprintf(L"  State:    %s\n", StateToString(task.State()));

    if (task.IconUri())
        wprintf(L"  Icon:     %s\n", task.IconUri().ToString().c_str());
    if (task.DeepLink())
        wprintf(L"  DeepLink: %s\n", task.DeepLink().ToString().c_str());

    wprintf(L"  Hidden:   %s\n", task.HiddenByUser() ? L"Yes" : L"No");
    wprintf(L"  Start:    %lld\n", task.StartTime().time_since_epoch().count());

    auto endTime = task.EndTime();
    if (endTime)
    {
        wprintf(L"  End:      %lld\n", endTime.Value().time_since_epoch().count());
    }
    else
    {
        wprintf(L"  End:      (not ended)\n");
    }
}

// 1. Check API Support
void CheckApiSupport()
{
    wprintf(L"\n=== Check API Support ===\n");
    bool supported = AppTaskInfo::IsSupported();
    wprintf(L"AppTaskInfo::IsSupported() = %s\n", supported ? L"true" : L"false");
}

// 2. Create Basic Task
void CreateBasicTask(TaskContext& ctx)
{
    wprintf(L"\n=== Create Basic Task ===\n");

    auto content = AppTaskContent::CreateSequenceOfSteps(
        { L"Initializing", L"Processing", L"Finalizing" },
        L"Starting basic task..."
    );

    auto iconUri = Uri(L"ms-appx:///Assets/icon.png");
    auto deepLink = Uri(L"ms-apptask:///open");

    auto task = AppTaskInfo::Create(
        L"Basic Test Task",
        L"A simple test of the AppTask API",
        iconUri,
        deepLink,
        content
    );

    ctx.currentTask = task;
    wprintf(L"Task created successfully!\n");
    PrintTaskInfo(task);
}

// 3. Create Task with Sequence of Steps
void CreateTaskWithSteps(TaskContext& ctx)
{
    wprintf(L"\n=== Create Task with Sequence of Steps ===\n");

    auto content = AppTaskContent::CreateSequenceOfSteps(
        { L"Downloading data", L"Parsing results", L"Generating report", L"Uploading output", L"Done" },
        L"Step 1: Downloading data..."
    );

    auto task = AppTaskInfo::Create(
        L"Multi-Step Task",
        L"Testing CreateSequenceOfSteps",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///steps"),
        content
    );

    ctx.currentTask = task;
    wprintf(L"Task with steps created!\n");
    PrintTaskInfo(task);
}

// 4. Create Task with Preview Thumbnail
void CreateTaskWithPreviewThumbnail(TaskContext& ctx)
{
    wprintf(L"\n=== Create Task with Preview Thumbnail ===\n");

    auto content = AppTaskContent::CreatePreviewThumbnail(
        Uri(L"ms-appx:///Assets/icon.png"),
        L"Preview of generated content"
    );

    auto task = AppTaskInfo::Create(
        L"Thumbnail Task",
        L"Testing CreatePreviewThumbnail",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///thumbnail"),
        content
    );

    ctx.currentTask = task;
    wprintf(L"Task with thumbnail created!\n");
    PrintTaskInfo(task);
}

// 5. Create Task with Text Summary Result
void CreateTaskWithTextSummary(TaskContext& ctx)
{
    wprintf(L"\n=== Create Task with Text Summary Result ===\n");

    auto content = AppTaskContent::CreateTextSummaryResult(
        L"Analysis complete: 42 items processed, 3 warnings, 0 errors."
    );

    auto task = AppTaskInfo::Create(
        L"Text Summary Task",
        L"Testing CreateTextSummaryResult",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///summary"),
        content
    );

    ctx.currentTask = task;
    wprintf(L"Task with text summary created!\n");
    PrintTaskInfo(task);
}

// 6. Create Task with Generated Assets Result
void CreateTaskWithGeneratedAssets(TaskContext& ctx)
{
    wprintf(L"\n=== Create Task with Generated Assets Result ===\n");

    AppTaskResultAsset asset1(
        L"Report.pdf",
        L"Monthly report document",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///asset/report")
    );

    AppTaskResultAsset asset2(
        L"Chart.png",
        L"Sales chart image",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///asset/chart")
    );

    auto content = AppTaskContent::CreateGeneratedAssetsResult({ asset1, asset2 });

    auto task = AppTaskInfo::Create(
        L"Generated Assets Task",
        L"Testing CreateGeneratedAssetsResult",
        Uri(L"ms-appx:///Assets/icon.png"),
        Uri(L"ms-apptask:///assets"),
        content
    );

    ctx.currentTask = task;
    wprintf(L"Task with generated assets created!\n");
    PrintTaskInfo(task);
}

// 7. Update Task State (cycle through all states)
void UpdateTaskState(TaskContext& ctx)
{
    wprintf(L"\n=== Update Task State ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    wprintf(L"Current state: %s\n", StateToString(ctx.currentTask.State()));
    wprintf(L"Select new state:\n");
    wprintf(L"  0 = Running\n");
    wprintf(L"  1 = Completed\n");
    wprintf(L"  2 = NeedsAttention\n");
    wprintf(L"  3 = Paused\n");
    wprintf(L"  4 = Error\n");
    wprintf(L"> ");

    wchar_t buf[8] = {};
    if (_getws_s(buf, _countof(buf)))
    {
        int choice = _wtoi(buf);
        if (choice >= 0 && choice <= 4)
        {
            auto newState = static_cast<AppTaskState>(choice);
            ctx.currentTask.UpdateState(newState);
            wprintf(L"State updated to: %s\n", StateToString(newState));
        }
        else
        {
            wprintf(L"Invalid choice.\n");
        }
    }
}

// 8. Update Task Titles
void UpdateTaskTitles(TaskContext& ctx)
{
    wprintf(L"\n=== Update Task Titles ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    wchar_t title[256] = {};
    wchar_t subtitle[256] = {};

    wprintf(L"Enter new title: ");
    _getws_s(title, _countof(title));

    wprintf(L"Enter new subtitle: ");
    _getws_s(subtitle, _countof(subtitle));

    ctx.currentTask.UpdateTitles(title, subtitle);
    wprintf(L"Titles updated!\n");
    wprintf(L"  Title:    %s\n", ctx.currentTask.Title().c_str());
    wprintf(L"  Subtitle: %s\n", ctx.currentTask.Subtitle().c_str());
}

// 9. Update Task Deep Link
void UpdateTaskDeepLink(TaskContext& ctx)
{
    wprintf(L"\n=== Update Task Deep Link ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    wchar_t uriStr[512] = {};
    wprintf(L"Enter new deep link URI: ");
    _getws_s(uriStr, _countof(uriStr));

    ctx.currentTask.UpdateDeepLink(Uri(uriStr));
    wprintf(L"Deep link updated to: %s\n", ctx.currentTask.DeepLink().ToString().c_str());
}

// 10. Update Task Content (state + new content)
void UpdateTaskContent(TaskContext& ctx)
{
    wprintf(L"\n=== Update Task Content (State + Content) ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    auto content = AppTaskContent::CreateTextSummaryResult(
        L"Updated result: task content was refreshed via Update()."
    );

    ctx.currentTask.Update(AppTaskState::Completed, content);
    wprintf(L"Task updated with new state (Completed) and text summary content.\n");
}

// 11. Add Interactive Elements
void AddInteractiveElements(TaskContext& ctx)
{
    wprintf(L"\n=== Add Interactive Elements ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    wprintf(L"Choose element to add:\n");
    wprintf(L"  1 = SetQuestion\n");
    wprintf(L"  2 = AddButton\n");
    wprintf(L"  3 = SetTextInput\n");
    wprintf(L"  4 = All of the above (create new task with all)\n");
    wprintf(L"> ");

    wchar_t buf[8] = {};
    if (!_getws_s(buf, _countof(buf)))
        return;

    int choice = _wtoi(buf);

    if (choice == 4)
    {
        // Create a fresh task with all interactive elements
        auto content = AppTaskContent::CreateSequenceOfSteps(
            { L"Waiting for input" },
            L"This task needs your attention"
        );

        content.SetQuestion(L"How would you like to proceed?");
        content.AddButton(L"Continue", Uri(L"ms-apptask:///action/continue"));
        content.AddButton(L"Cancel", Uri(L"ms-apptask:///action/cancel"));
        content.SetTextInput(L"Or type your response:", L"Type here...");

        auto task = AppTaskInfo::Create(
            L"Interactive Task",
            L"Testing interactive elements",
            Uri(L"ms-appx:///Assets/icon.png"),
            Uri(L"ms-apptask:///interactive"),
            content
        );

        task.UpdateState(AppTaskState::NeedsAttention);
        ctx.currentTask = task;
        wprintf(L"Interactive task created with question, buttons, and text input!\n");
        PrintTaskInfo(task);
    }
    else
    {
        // Apply to existing task via Update
        auto content = AppTaskContent::CreateSequenceOfSteps(
            { L"Waiting for input" },
            L"Updated with interactive element"
        );

        switch (choice)
        {
        case 1:
            content.SetQuestion(L"Should we continue processing?");
            wprintf(L"Question set on content.\n");
            break;
        case 2:
            content.AddButton(L"Approve", Uri(L"ms-apptask:///approve"));
            wprintf(L"Button added to content.\n");
            break;
        case 3:
            content.SetTextInput(L"Enter your feedback:", L"Type here...");
            wprintf(L"Text input set on content.\n");
            break;
        default:
            wprintf(L"Invalid choice.\n");
            return;
        }

        ctx.currentTask.Update(AppTaskState::NeedsAttention, content);
        wprintf(L"Task updated with interactive content.\n");
    }
}

// 12. Enumerate Tasks
void EnumerateTasks()
{
    wprintf(L"\n=== Enumerate All Tasks ===\n");

    auto tasks = AppTaskInfo::FindAll();
    uint32_t count = static_cast<uint32_t>(tasks.size());
    wprintf(L"Found %u task(s):\n", count);

    for (uint32_t i = 0; i < count; i++)
    {
        wprintf(L"\n--- Task %u ---\n", i + 1);
        PrintTaskInfo(tasks[i]);
    }

    if (count == 0)
    {
        wprintf(L"(No tasks found)\n");
    }
}

// 13. Inspect Task Details
void InspectTaskDetails(TaskContext& ctx)
{
    wprintf(L"\n=== Inspect Task Details ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task. Create one first.\n");
        return;
    }

    wprintf(L"Task Properties:\n");
    PrintTaskInfo(ctx.currentTask);

    wprintf(L"\nExecuting Step:\n");
    auto executingStep = ctx.currentTask.GetExecutingStep();
    if (!executingStep.empty())
    {
        wprintf(L"  %s\n", executingStep.c_str());
    }
    else
    {
        wprintf(L"  (none)\n");
    }

    wprintf(L"\nCompleted Steps:\n");
    auto completedSteps = ctx.currentTask.GetCompletedSteps();
    if (completedSteps.size() > 0)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(completedSteps.size()); i++)
        {
            wprintf(L"  [%u] %s\n", i + 1, completedSteps[i].c_str());
        }
    }
    else
    {
        wprintf(L"  (none)\n");
    }
}

// 14. Remove Current Task
void RemoveCurrentTask(TaskContext& ctx)
{
    wprintf(L"\n=== Remove Current Task ===\n");

    if (!ctx.currentTask)
    {
        wprintf(L"No current task to remove.\n");
        return;
    }

    hstring id = ctx.currentTask.Id();
    ctx.currentTask.Remove();
    ctx.currentTask = nullptr;
    wprintf(L"Task '%s' removed.\n", id.c_str());
}

// 15. Remove All Tasks
void RemoveAllTasks()
{
    wprintf(L"\n=== Remove All Tasks ===\n");

    auto tasks = AppTaskInfo::FindAll();
    uint32_t count = static_cast<uint32_t>(tasks.size());

    for (uint32_t i = 0; i < count; i++)
    {
        auto& task = tasks[i];
        hstring id = task.Id();
        task.Remove();
        wprintf(L"  Removed: %s\n", id.c_str());
    }

    wprintf(L"Removed %u task(s).\n", count);
}
