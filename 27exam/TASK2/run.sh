#指定bash运行这个文件
#!/usr/bin/env bash
cd -- "$(dirname -- "$0")" || exit 1
build_project() {
    #cmake检测到文件变化才会调用构建
    #cmake构建工具调用前的时间
    local before=""
    #cmake构建工具调用后时间
    local after=""
    #对比前后文件是否有区别
    if [ -f ./app ]; then
        before=$(stat -c '%y' ./app) || return
    fi
    cmake -S . -B ./build >&2 || return
    #"--build"表示调用cmake的build完成编译并生成文件
    cmake --build ./build >&2 || return
    after=$(stat -c '%y' ./app) || return
    if [ "$before" = "$after" ]; then
        echo "程序已是最新，跳过编译" >&2
    else
        echo "编译完成" >&2
    fi
}
case "$1" in
    build)
        build_project
        ;;
    run)
        #移除第一个run
        shift
        #如果第二个参数是--，也移除
        if [ "${1:-}" = "--" ]; then
            shift
        fi
        #调用build_project，如果成功，执行app
        build_project && ./app "$@"
        ;;
    clean)
        #"--"表示接下来的语句不是指令，防止以'-'开头的文件被识别成命令
        rm -rf -- ./build &&
        rm -f -- ./app &&
        echo "清理完成"
        ;;
    help)
        echo "指令: build/run/clean/help"
        ;;
    *)
        echo "未知命令，请使用 ./run.sh help" >&2
        exit 1
        ;;
esac
