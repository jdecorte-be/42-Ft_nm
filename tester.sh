#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "❌ Usage: $0 <directory>"
    exit 1
fi

DIR="$1"
LOG_DIR="/tmp/nm_logs"
mkdir -p "$LOG_DIR"

cleanup() {
    rm -rf "$LOG_DIR"
}

trap cleanup EXIT

if [[ ! -d "$DIR" ]]; then
    echo "❌ Directory $DIR does not exist!"
    exit 1
fi

for file in $(find "$DIR" -type f -executable); do
    FILE_LOG_DIR="$LOG_DIR/$(basename "$file")_logs"
    mkdir -p "$FILE_LOG_DIR"

    NM_OUTPUT="$FILE_LOG_DIR/nm_output.txt"
    FT_NM_OUTPUT="$FILE_LOG_DIR/ft_nm_output.txt"
    VALGRIND_OUTPUT="$FILE_LOG_DIR/valgrind_output.txt"

    echo "🔍 Testing $file..."

    nm "$file" > "$NM_OUTPUT" 2>&1
    NM_EXIT_STATUS=$?

    ./ft_nm "$file" > "$FT_NM_OUTPUT" 2>&1
    FT_NM_EXIT_STATUS=$?

    if [[ $NM_EXIT_STATUS != 0 && $FT_NM_EXIT_STATUS != 0 ]]; then
        echo "✅⚠️  Both commands produced an error for $file"
        continue
    fi

    valgrind --error-exitcode=100 -s --log-file="$VALGRIND_OUTPUT" ./ft_nm "$file" > /dev/null 2>&1
    FT_NM_VALGRIND_EXIT_STATUS=$?

    if [[ $FT_NM_VALGRIND_EXIT_STATUS == 100 ]]; then
        echo "❌ Valgrind reported errors for $file"
        cat "$VALGRIND_OUTPUT"
    fi

    if diff -q "$NM_OUTPUT" "$FT_NM_OUTPUT" > /dev/null; then
        echo "✅ $file is OK"
    else
        echo "❌ Difference detected for $file"
        diff "$NM_OUTPUT" "$FT_NM_OUTPUT"
    fi
done
