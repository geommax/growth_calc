

$ gcc example.c

$ ./a.out

$ cat example.c

$ gcc --version 

Before Compilation -> do preprocess only -> $ gcc -E example.c -o example.i 

generate assembly code  -> $ gcc -S example.i -o example.s 

From Assembly to object - Compile only - $ gcc -c example.s -o example.s 

generate executable file - > $ gcc example.o -o a.out



https://en.wikipedia.org/wiki/GNU_toolchain

GDB - GNU Debugger.

$ g++ --version 

------------------------------

gcc version ချိန်းချင်ရင် container သုံးပြီးရှင်းသင့်ကြောင်းပြ။ 

-- default local စက်ထဲက version ကိုပဲသုံးချင်တယ်ဆိုရင်၊ 

$ sudo apt install g++-version 

$ sudo update-alternatives --config gcc 

(example)
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 

-------------------------------
---------------------------------


gdb -q a.out 

> r (run)

> b (break)

> n (next)

> c (continue)

--------------------------

Makefile ဆိုတာ Linux Shell commands တွေပါတဲ့ ဖိုင်အမျိးအစား တစ်ခုပါပဲ ။ 


---------------------------------
---------------------------------
sudo apt install build-essential
g++ → C++ compiler

gcc → C compiler

make → build tool

libstdc++ → C++ standard library

-----------------------------------

sudo apt install cmake
CMake က Makefile / Ninja / VS project တွေကို generate လုပ်ပေးတဲ့ build system generator ဖြစ်ပါတယ်


------------------------------
GDB (GNU Debugger)
sudo apt install gdb

breakpoint

step by step execution

crash analysis

----------------------------


sudo apt install git
source code history

teamwork

GitHub / GitLab

--------------------------------
CMake နဲ့တွဲသုံး

sudo apt install ninja-build
cmake -G Ninja ..
ninja


-----------------------------

sudo apt install clang
Alternative compiler (LLVM based)

---------------------------

sudo apt install build-essential cmake gdb git


----------------------------------------
-------------------------------------------

Not Stripped (Debug Binary)
debug symbols ပါတယ်

function name, variable name, source line info ရှိ

gdb နဲ့ line-by-line debug လုပ်လို့ရ
ါဝင်တဲ့ info တွေ

function names

source file names

line numbers

local variables

-----------------------------------------
Stripped (Release / Production Binary)

debug symbols ဖယ်ထားတယ်

binary size သေး

reverse / debug လုပ်ရခက်

မပါတာတွေ

function names 

line numbers 

variable names

----------------------------
Testing

---------------------
$ g++ -g main.cpp -o app_debug

$ strip app_debug -o app_stripped

readelf -S app_debug

gdb ./app_debug

break main
run
list
info locals

source code မြင်ရမယ်

variable name တွေ ထွက်မယ်

line number နဲ့ stop မယ်


-------------------------------

cmake နဲ့ build တဲ့ အခါ debug symbol ပါစေချင်ရင်။ 

cmake -DCMAKE_BUILD_TYPE=Debug ..
make