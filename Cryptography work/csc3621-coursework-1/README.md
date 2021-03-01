# CSC3621 Coursework quick-start

If (you think that) you master Java, you may not need to read from this file.
Otherwise, you are encouraged to read it through.

## Documentation

You should have been provided of a `doc/` directory with the necessary documentation about the partial source code that you are asked to complete.

You can consult it with your favourite web browser opening the `doc/index.html` file.

You must not change the provided methods outside the following comments:

- `//<editor-fold defaultstate="collapsed" desc="Write your code below">`
- `//</editor-fold> // END OF YOUR CODE`

Not even the signatures.
Yet, you are allowed to create additional reasonable supporting methods and classes.

## How to edit the source

You can either

- use an IDE of your choice (Netbeans, Eclipse, ...) or
- edit the files in your favourite text editor.

To test your progress you can compile your source and run it.

- Depending on the IDE, once you have copied the directory structure into your project, they have facilities to compile and run pushing buttons (we do not explain this, because it is usually easy to set-up).
- You can decide to compile and run by your own with a shell (command prompt).

## Suggestions to compile and run from the shell

You need to have `javac` and `java` available in your shell.
They usually come from any installation of JDK.

### Linux

Once you changed the directory to where this file is, you can create a build directory so you do not mix source code with compiled code (you need to submit java files; class file will be ignored).

    mkdir -p build
    cd build

You may need to copy the coursework input files too, depending on your implementation.
We strongly recommend to use resources; marks may be deducted if you code requires manual intervention either to compile or to run.

    cp -r ../src/res .

To compile the code you can write

    javac -d . ../src/uk/ac/ncl/undergraduate/modules/csc3621/cryptanalysis/easyfreq/*.java

If it compiles without errors, you can run the classes with a main inside (FrequencyCryptanalysis or VigenereCryptanalysis).
For example,

    java uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq.FrequencyCryptanalysis

### Windows

If `javac` is not available when you open command prompt, you can add its containing directory (something like `C:\Program Files\Java\jdk1.8.0_181\bin`) among the list of the environment variable `Path`.
Then, close and re-open the command prompt and check again.


Once you changed the directory to where this file is, you can create a build directory so you do not mix source code with compiled code (you need to submit java files; class file will be ignored).

    md build
    cd build

You may need to copy the coursework input files too, depending on your implementation.
We strongly recommend to use resources; marks may be deducted if you code requires manual intervention either to compile or to run.

    md res
    copy ../src/res res

To compile the code you can write

    javac -d . ..\src\uk\ac\ncl\undergraduate\modules\csc3621\cryptanalysis\easyfreq\*.java

If it compiles without errors, you can run the classes with a main inside (FrequencyCryptanalysis or VigenereCryptanalysis).
For example,

    java uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq.FrequencyCryptanalysis

