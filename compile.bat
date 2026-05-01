echo text_analyzer building...
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
echo complete!

:: example
:: ./text_analyzer "C:\Users\Max\Documents\final_project\" "in" 3