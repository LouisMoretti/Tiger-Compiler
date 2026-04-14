; ModuleID = 'src/llvmtranslate/tiger-runtime.c'
source_filename = "src/llvmtranslate/tiger-runtime.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@tc_malloc.msg = internal constant [26 x i8] c"malloc: memory exhausted\0A\00", align 16
@stderr = external global ptr, align 8
@.str = private unnamed_addr constant [29 x i8] c"chr: character out of range\0A\00", align 1
@consts = internal global [512 x i8] zeroinitializer, align 16
@.str.1 = private unnamed_addr constant [36 x i8] c"substring: arguments out of bounds\0A\00", align 1
@stdin = external global ptr, align 8
@.str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"%ld\00", align 1
@stdout = external global ptr, align 8

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local void @gc_enter_runtime() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local void @gc_exit_runtime() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local ptr @tc_malloc(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  %3 = alloca ptr, align 8
  store i64 %0, ptr %2, align 8
  %4 = load i64, ptr %2, align 8
  %5 = call noalias ptr @malloc(i64 noundef %4) #6
  store ptr %5, ptr %3, align 8
  %6 = load ptr, ptr %3, align 8
  %7 = icmp eq ptr %6, null
  br i1 %7, label %8, label %11

8:                                                ; preds = %1
  %9 = load ptr, ptr @stderr, align 8
  %10 = call i64 @fwrite(ptr noundef @tc_malloc.msg, i64 noundef 1, i64 noundef 25, ptr noundef %9)
  call void @exit(i32 noundef 120) #7
  unreachable

11:                                               ; preds = %1
  %12 = load ptr, ptr %3, align 8
  %13 = load i64, ptr %2, align 8
  call void @llvm.memset.p0.i64(ptr align 1 %12, i8 0, i64 %13, i1 false)
  %14 = load ptr, ptr %3, align 8
  ret ptr %14
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

declare i64 @fwrite(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #2

; Function Attrs: noreturn nounwind
declare void @exit(i32 noundef) #3

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local ptr @tc_init_array(i64 noundef %0, i64 noundef %1) #0 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  store i64 %0, ptr %3, align 8
  store i64 %1, ptr %4, align 8
  %7 = load i64, ptr %3, align 8
  %8 = mul i64 %7, 8
  %9 = call ptr @tc_malloc(i64 noundef %8)
  store ptr %9, ptr %5, align 8
  store i64 0, ptr %6, align 8
  br label %10

10:                                               ; preds = %19, %2
  %11 = load i64, ptr %6, align 8
  %12 = load i64, ptr %3, align 8
  %13 = icmp ult i64 %11, %12
  br i1 %13, label %14, label %22

14:                                               ; preds = %10
  %15 = load i64, ptr %4, align 8
  %16 = load ptr, ptr %5, align 8
  %17 = load i64, ptr %6, align 8
  %18 = getelementptr inbounds i64, ptr %16, i64 %17
  store i64 %15, ptr %18, align 8
  br label %19

19:                                               ; preds = %14
  %20 = load i64, ptr %6, align 8
  %21 = add i64 %20, 1
  store i64 %21, ptr %6, align 8
  br label %10

22:                                               ; preds = %10
  %23 = load ptr, ptr %5, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local ptr @tc_init_ptr_array(i64 noundef %0, i64 noundef %1) #0 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %0, ptr %3, align 8
  store i64 %1, ptr %4, align 8
  %5 = load i64, ptr %3, align 8
  %6 = load i64, ptr %4, align 8
  %7 = call ptr @tc_init_array(i64 noundef %5, i64 noundef %6)
  ret ptr %7
}

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local ptr @tc_init_string(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = call ptr @tc_malloc(i64 noundef %3)
  ret ptr %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @tc_not(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = icmp ne i64 %3, 0
  %5 = xor i1 %4, true
  %6 = zext i1 %5 to i32
  %7 = sext i32 %6 to i64
  ret i64 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_exit(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = trunc i64 %3 to i32
  call void @exit(i32 noundef %4) #7
  unreachable
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @tc_chr(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = icmp sle i64 0, %3
  br i1 %4, label %5, label %8

5:                                                ; preds = %1
  %6 = load i64, ptr %2, align 8
  %7 = icmp sle i64 %6, 255
  br i1 %7, label %11, label %8

8:                                                ; preds = %5, %1
  %9 = load ptr, ptr @stderr, align 8
  %10 = call i32 @fputs(ptr noundef @.str, ptr noundef %9)
  call void @exit(i32 noundef 120) #7
  unreachable

11:                                               ; preds = %5
  %12 = load i64, ptr %2, align 8
  %13 = mul nsw i64 %12, 2
  %14 = getelementptr inbounds i8, ptr @consts, i64 %13
  ret ptr %14
}

declare i32 @fputs(ptr noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @tc_concat(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %11 = load ptr, ptr %4, align 8
  %12 = call i64 @strlen(ptr noundef %11) #8
  store i64 %12, ptr %6, align 8
  %13 = load ptr, ptr %5, align 8
  %14 = call i64 @strlen(ptr noundef %13) #8
  store i64 %14, ptr %7, align 8
  %15 = load i64, ptr %6, align 8
  %16 = icmp eq i64 %15, 0
  br i1 %16, label %17, label %19

17:                                               ; preds = %2
  %18 = load ptr, ptr %5, align 8
  store ptr %18, ptr %3, align 8
  br label %69

19:                                               ; preds = %2
  %20 = load i64, ptr %7, align 8
  %21 = icmp eq i64 %20, 0
  br i1 %21, label %22, label %24

22:                                               ; preds = %19
  %23 = load ptr, ptr %4, align 8
  store ptr %23, ptr %3, align 8
  br label %69

24:                                               ; preds = %19
  store i64 0, ptr %8, align 8
  %25 = load i64, ptr %6, align 8
  %26 = load i64, ptr %7, align 8
  %27 = add i64 %25, %26
  store i64 %27, ptr %9, align 8
  %28 = load i64, ptr %9, align 8
  %29 = add i64 %28, 1
  %30 = call ptr @tc_init_string(i64 noundef %29)
  store ptr %30, ptr %10, align 8
  store i64 0, ptr %8, align 8
  br label %31

31:                                               ; preds = %43, %24
  %32 = load i64, ptr %8, align 8
  %33 = load i64, ptr %6, align 8
  %34 = icmp ult i64 %32, %33
  br i1 %34, label %35, label %46

35:                                               ; preds = %31
  %36 = load ptr, ptr %4, align 8
  %37 = load i64, ptr %8, align 8
  %38 = getelementptr inbounds i8, ptr %36, i64 %37
  %39 = load i8, ptr %38, align 1
  %40 = load ptr, ptr %10, align 8
  %41 = load i64, ptr %8, align 8
  %42 = getelementptr inbounds i8, ptr %40, i64 %41
  store i8 %39, ptr %42, align 1
  br label %43

43:                                               ; preds = %35
  %44 = load i64, ptr %8, align 8
  %45 = add i64 %44, 1
  store i64 %45, ptr %8, align 8
  br label %31

46:                                               ; preds = %31
  store i64 0, ptr %8, align 8
  br label %47

47:                                               ; preds = %61, %46
  %48 = load i64, ptr %8, align 8
  %49 = load i64, ptr %7, align 8
  %50 = icmp ult i64 %48, %49
  br i1 %50, label %51, label %64

51:                                               ; preds = %47
  %52 = load ptr, ptr %5, align 8
  %53 = load i64, ptr %8, align 8
  %54 = getelementptr inbounds i8, ptr %52, i64 %53
  %55 = load i8, ptr %54, align 1
  %56 = load ptr, ptr %10, align 8
  %57 = load i64, ptr %8, align 8
  %58 = load i64, ptr %6, align 8
  %59 = add i64 %57, %58
  %60 = getelementptr inbounds i8, ptr %56, i64 %59
  store i8 %55, ptr %60, align 1
  br label %61

61:                                               ; preds = %51
  %62 = load i64, ptr %8, align 8
  %63 = add i64 %62, 1
  store i64 %63, ptr %8, align 8
  br label %47

64:                                               ; preds = %47
  %65 = load ptr, ptr %10, align 8
  %66 = load i64, ptr %9, align 8
  %67 = getelementptr inbounds i8, ptr %65, i64 %66
  store i8 0, ptr %67, align 1
  %68 = load ptr, ptr %10, align 8
  store ptr %68, ptr %3, align 8
  br label %69

69:                                               ; preds = %64, %22, %17
  %70 = load ptr, ptr %3, align 8
  ret ptr %70
}

; Function Attrs: nounwind willreturn memory(read)
declare i64 @strlen(ptr noundef) #5

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @tc_ord(ptr noundef %0) #0 {
  %2 = alloca i64, align 8
  %3 = alloca ptr, align 8
  %4 = alloca i64, align 8
  store ptr %0, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = call i64 @strlen(ptr noundef %5) #8
  store i64 %6, ptr %4, align 8
  %7 = load i64, ptr %4, align 8
  %8 = icmp eq i64 %7, 0
  br i1 %8, label %9, label %10

9:                                                ; preds = %1
  store i64 -1, ptr %2, align 8
  br label %15

10:                                               ; preds = %1
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds i8, ptr %11, i64 0
  %13 = load i8, ptr %12, align 1
  %14 = sext i8 %13 to i64
  store i64 %14, ptr %2, align 8
  br label %15

15:                                               ; preds = %10, %9
  %16 = load i64, ptr %2, align 8
  ret i64 %16
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @tc_size(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i64 @strlen(ptr noundef %3) #8
  ret i64 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @tc_substring(ptr noundef %0, i64 noundef %1, i64 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca ptr, align 8
  %10 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store i64 %1, ptr %6, align 8
  store i64 %2, ptr %7, align 8
  %11 = load ptr, ptr %5, align 8
  %12 = call i64 @strlen(ptr noundef %11) #8
  store i64 %12, ptr %8, align 8
  %13 = load i64, ptr %6, align 8
  %14 = icmp sle i64 0, %13
  br i1 %14, label %15, label %24

15:                                               ; preds = %3
  %16 = load i64, ptr %7, align 8
  %17 = icmp sle i64 0, %16
  br i1 %17, label %18, label %24

18:                                               ; preds = %15
  %19 = load i64, ptr %6, align 8
  %20 = load i64, ptr %7, align 8
  %21 = add nsw i64 %19, %20
  %22 = load i64, ptr %8, align 8
  %23 = icmp ule i64 %21, %22
  br i1 %23, label %27, label %24

24:                                               ; preds = %18, %15, %3
  %25 = load ptr, ptr @stderr, align 8
  %26 = call i32 @fputs(ptr noundef @.str.1, ptr noundef %25)
  call void @exit(i32 noundef 120) #7
  unreachable

27:                                               ; preds = %18
  %28 = load i64, ptr %7, align 8
  %29 = icmp eq i64 %28, 1
  br i1 %29, label %30, label %39

30:                                               ; preds = %27
  %31 = load ptr, ptr %5, align 8
  %32 = load i64, ptr %6, align 8
  %33 = getelementptr inbounds i8, ptr %31, i64 %32
  %34 = load i8, ptr %33, align 1
  %35 = sext i8 %34 to i32
  %36 = mul nsw i32 %35, 2
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds i8, ptr @consts, i64 %37
  store ptr %38, ptr %4, align 8
  br label %68

39:                                               ; preds = %27
  %40 = load i64, ptr %7, align 8
  %41 = add nsw i64 %40, 1
  %42 = call ptr @tc_init_string(i64 noundef %41)
  store ptr %42, ptr %9, align 8
  store i32 0, ptr %10, align 4
  br label %43

43:                                               ; preds = %60, %39
  %44 = load i32, ptr %10, align 4
  %45 = sext i32 %44 to i64
  %46 = load i64, ptr %7, align 8
  %47 = icmp slt i64 %45, %46
  br i1 %47, label %48, label %63

48:                                               ; preds = %43
  %49 = load ptr, ptr %5, align 8
  %50 = load i64, ptr %6, align 8
  %51 = load i32, ptr %10, align 4
  %52 = sext i32 %51 to i64
  %53 = add nsw i64 %50, %52
  %54 = getelementptr inbounds i8, ptr %49, i64 %53
  %55 = load i8, ptr %54, align 1
  %56 = load ptr, ptr %9, align 8
  %57 = load i32, ptr %10, align 4
  %58 = sext i32 %57 to i64
  %59 = getelementptr inbounds i8, ptr %56, i64 %58
  store i8 %55, ptr %59, align 1
  br label %60

60:                                               ; preds = %48
  %61 = load i32, ptr %10, align 4
  %62 = add nsw i32 %61, 1
  store i32 %62, ptr %10, align 4
  br label %43

63:                                               ; preds = %43
  %64 = load ptr, ptr %9, align 8
  %65 = load i64, ptr %7, align 8
  %66 = getelementptr inbounds i8, ptr %64, i64 %65
  store i8 0, ptr %66, align 1
  %67 = load ptr, ptr %9, align 8
  store ptr %67, ptr %4, align 8
  br label %68

68:                                               ; preds = %63, %30
  %69 = load ptr, ptr %4, align 8
  ret ptr %69
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @tc_strcmp(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = call i32 @strcmp(ptr noundef %5, ptr noundef %6) #8
  %8 = sext i32 %7 to i64
  ret i64 %8
}

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strcmp(ptr noundef, ptr noundef) #5

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @tc_streq(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = call i32 @strcmp(ptr noundef %5, ptr noundef %6) #8
  %8 = icmp eq i32 %7, 0
  %9 = zext i1 %8 to i32
  %10 = sext i32 %9 to i64
  ret i64 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @tc_getchar() #0 {
  %1 = alloca ptr, align 8
  %2 = alloca i32, align 4
  %3 = load ptr, ptr @stdin, align 8
  %4 = call i32 @getc(ptr noundef %3)
  store i32 %4, ptr %2, align 4
  %5 = load i32, ptr %2, align 4
  %6 = icmp eq i32 %5, -1
  br i1 %6, label %7, label %8

7:                                                ; preds = %0
  store ptr @.str.2, ptr %1, align 8
  br label %13

8:                                                ; preds = %0
  %9 = load i32, ptr %2, align 4
  %10 = mul nsw i32 %9, 2
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds i8, ptr @consts, i64 %11
  store ptr %12, ptr %1, align 8
  br label %13

13:                                               ; preds = %8, %7
  %14 = load ptr, ptr %1, align 8
  ret ptr %14
}

declare i32 @getc(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_print(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, ptr noundef %3)
  ret void
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_print_err(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr @stderr, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %3, ptr noundef @.str.3, ptr noundef %4)
  ret void
}

declare i32 @fprintf(ptr noundef, ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_print_int(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i64 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @tc_flush() #0 {
  %1 = load ptr, ptr @stdout, align 8
  %2 = call i32 @fflush(ptr noundef %1)
  ret void
}

declare i32 @fflush(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @init_consts() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  br label %2

2:                                                ; preds = %16, %0
  %3 = load i32, ptr %1, align 4
  %4 = icmp slt i32 %3, 512
  br i1 %4, label %5, label %19

5:                                                ; preds = %2
  %6 = load i32, ptr %1, align 4
  %7 = sdiv i32 %6, 2
  %8 = trunc i32 %7 to i8
  %9 = load i32, ptr %1, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [512 x i8], ptr @consts, i64 0, i64 %10
  store i8 %8, ptr %11, align 1
  %12 = load i32, ptr %1, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [512 x i8], ptr @consts, i64 0, i64 %14
  store i8 0, ptr %15, align 1
  br label %16

16:                                               ; preds = %5
  %17 = load i32, ptr %1, align 4
  %18 = add nsw i32 %17, 2
  store i32 %18, ptr %1, align 4
  br label %2

19:                                               ; preds = %2
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define weak dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  call void @init_consts()
  call void @tc_main(i32 noundef 0)
  ret i32 0
}

declare void @tc_main(i32 noundef) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind allocsize(0) }
attributes #7 = { noreturn nounwind }
attributes #8 = { nounwind willreturn memory(read) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.1.8"}
