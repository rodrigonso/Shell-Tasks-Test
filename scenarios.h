#pragma once
#include "pch.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Shell::Tasks;

// Holds the last created task so subsequent operations can act on it.
struct TaskContext
{
    AppTaskInfo currentTask{ nullptr };
};

void CheckApiSupport();
void CreateBasicTask(TaskContext& ctx);
void CreateTaskWithSteps(TaskContext& ctx);
void CreateTaskWithPreviewThumbnail(TaskContext& ctx);
void CreateTaskWithTextSummary(TaskContext& ctx);
void CreateTaskWithGeneratedAssets(TaskContext& ctx);
void UpdateTaskState(TaskContext& ctx);
void UpdateTaskTitles(TaskContext& ctx);
void UpdateTaskDeepLink(TaskContext& ctx);
void UpdateTaskContent(TaskContext& ctx);
void AddInteractiveElements(TaskContext& ctx);
void EnumerateTasks();
void InspectTaskDetails(TaskContext& ctx);
void RemoveCurrentTask(TaskContext& ctx);
void RemoveAllTasks();
