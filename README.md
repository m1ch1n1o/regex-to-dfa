# DFA generator

[//]: # (### Deterministic Finite Automaton Generator and converter C++)

### Requirements
* a modern C++17 compiler (`gcc-8`, `clang-6.0`, `MSVC 2017` or above)
* [`cmake`](https://cmake.org) 3.15+

### How to build from command line



```shell
  g++ build.cpp
```

```shell
  ./a.out 
```

### input / output syntax 

#### A regex is described like: 

> The language exclusively consists of lowercase Latin letters.
>
> The symbol '|' denotes union, allowing for combining multiple patterns.
>
> The asterisk '*' signifies repetition in regular expressions, permitting the repetition of the
> preceding pattern zero or more times.

#### A deterministic finite automaton M is described like: 

> The first line of the output consists of three space-separated numbers: 'n,' 'a,' and 't.' These numbers respectively
> represent the total number of states, the number of accepting states, and the total number of transitions in the
> finite
> automaton. It's important to note that states are numbered starting from 0, with state 0 always designated as the
> initial state.

> The second line contains 'a' numbers, which represent the indexes of the accepting states. In other words, there are '
> a' values listed on the second line, each indicating the index of an accepting state.

> Following this, each subsequent line describes the transitions within the finite automaton. Each of these lines begins
> with a number 'k<sub>i</sub>,' indicating the number of transitions originating from state 'i.' After 'k<sub>i</sub>,'
> pairs of two numbers
> are provided, each pair consisting of a symbol and the target state to which the automaton transitions from state 'i'
> using that symbol.


#### input example:

```
    (ab*c(0|1)*)*
```

#### output example:

```
    3 2 6
    0 2
    1 a 1
    2 b 1 c 2
    3 0 2 1 2 a 1
```


