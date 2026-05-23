; ModuleID = 'src/llvmgc/runtime/gc-ctrl.c'
source_filename = "src/llvmgc/runtime/gc-ctrl.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.gc_ctx = type { ptr, ptr, i8 }

@gc_ctx_ = external global %struct.gc_ctx, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_gc_enable(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = icmp ne i64 %3, 0
  %5 = xor i1 %4, true
  %6 = xor i1 %5, true
  %7 = zext i1 %6 to i8
  store i8 %7, ptr getelementptr inbounds (%struct.gc_ctx, ptr @gc_ctx_, i32 0, i32 2), align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_gc_run() #0 {
  call void @gc_enter_runtime()
  call void @gc_collect()
  call void @gc_exit_runtime()
  ret void
}

declare void @gc_enter_runtime() #1

declare void @gc_collect() #1

declare void @gc_exit_runtime() #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.1.8"}
