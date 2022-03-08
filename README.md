# Bite
![Last Commit](https://img.shields.io/github/last-commit/ProgrammingNerdGit/Bite)
![GitHub Repo stars](https://img.shields.io/github/stars/ProgrammingNerdGit/Bite?style=social)

# Contents

* [Goals](#goals)
* [Questions?](#questions)
* [Bug or Issue?](#bug-or-issue)
* [VSCode Syntax Highlighting](#vscode-syntax-highlighting)
* [Standard](#standard)
* [Compiling the Project (w/ CMake)](#compiling-the-project-w-cmake)
* [Code Examples / Format](#code-examples--format)


# Goals
These are my main design goals:

* #### __Versatility__. My goal with this project is to write a single program in Bite that can be converted into C or Python or anything you want. It could be used for anything and everything. As well as useful to individuals too, instead of having to write an entire language, you could use Bite as a template and easily modify it to do whatever you want!

* #### __Efficency__. The speed of a programming language is one of the most important factors to me. A slow language is a bad language (no offense). I mean how else are you going to run a 100000+ layer neural network?

* #### __Open Source__. This isn't much of a design goal, but it is important. This isn't open source so I can have 300 people contribute towards it, it's open-source so people can learn from it or fork it and make it their own. Although that being said I won't be accepting many pull requests to the base repo. I will take feedback and recommendations tho!

# Questions?
Before asking a question, look at the [Q&A](https://github.com/ProgrammingNerdGit/Bite/discussions/categories/q-a) to see if it's already been answered.

If not feel free to ask away [here](https://github.com/ProgrammingNerdGit/Bite/discussions/new?category=q-a), I won't bite ;)

# Bug or Issue?
There are probably many, first check if it's already reported. If not make an [issue](https://github.com/ProgrammingNerdGit/Bite/issues/new).

# VSCode Syntax Highlighting
If you want syntax highlighting for VSCode here's one from my [other repo](https://github.com/ProgrammingNerdGit/VSCode-Bite-Syntax). It probably could be better, but it works.

# Standard
I'm an advocate for coding how you want, the only thing I ask is to be consistent and use either, `camelCase`, `PascalCase`, or `MACRO_CASE`. `snake_case` is also fine.

And what I mean by "be consistent" is to use the same case for all functions, the same case for all variables (except constants if you are into that), etc. Functions and variables can be a different case, just all the functions and variables should be uniform in their own right.

## __Good practices will be listed in the docs__
but remember, DONT LET ANYONE TELL YOU HOW TO CODE, OR WHAT CODE IS 'GOOD' or 'BAD', those people are not welcome here (jk ofc).

# Compiling the Project (w/ CMake)
Compiling is super easy thanks to `CMake`, and trust me I didn't like it at first either, but after I gave it a fair shot I fell in love.

### __Example__:

in terminal/ cmd do the following commands (in the root of the project)

```
mkdir build
cd build
cmake ..
make # only run this if you compiled to makefile
```

and boom, you just compiled the project! now the binary will be in the `bin` folder.

# Code Examples / Format

so far I'm using the following as a reference to build the compiler, this is also [here](https://github.com/ProgrammingNerdGit/Bite/blob/dev/test/format.bite).
```Bite
!> resource relative to the global library dirrectory
resource `std/io.bite`;

!> resource to local library
resource fiz.bite;

!> this is a test comment
!*
    MULTI LINE COMMENT
*!

!> function definition, can return any type
func add(int32 a, int32 b) {
    return (a+b);
}

!> entry point, returns int32
func entry() -> int32 {
    int32 status = 0;
    str stringInit = "This is a string :)";
    return(status);
}
```