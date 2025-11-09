#!/bin/bash
cd "$(dirname "$0")"


while [ ! -f "CMakeLists.txt" ] && [ "$PWD" != "/" ]; do
  cd ..
done

if [ ! -f "CMakeLists.txt" ]; then
  echo "Error: CMakeLists.txt not found. Please run this script inside the project tree."
  exit 1
fi

echo "Formatting sources under: $(pwd)"

find src include \
  \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) \
  -exec clang-format -i {} +

echo "Formatting complete."
