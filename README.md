# Polya & Orbit Counting

An implementation of the Orbit Counting Theorem (Burnside's Lemma) and the Polya Enumeration Theorem for counting colourings under symmetry

Let $G$ be a finite group acting on a finite set $X$.

$$|X/G|=\dfrac{1}{|G|}\sum_{g\in G}|X^g|$$

where $X^g$ is the set of elements in $X$ fixed by $g$.

Let

$$Z_G(t_1, t_2, \ldots, t_{|X|})=\dfrac{1}{|G|}\sum_{g\in G}\prod_{k=1}^{\left|X\right|} t_k^{c_k(g)}$$

where $c_k(g)$ is the number of $k$-cycles of $g$ as a permutation of $X$ and

$$f(t)=f_1t+f_2t^2+\ldots+f_Kt^K.$$

Then in

$$Z_G\left(f(t),\ f(t^2),\ \ldots,\ f(t^{|X|})\right),$$

the coefficient of $f_1^{a_1} f_2^{a_2} \cdots f_K^{a_K}$ counts the number of distinct colourings using exactly $a_j$ copies of colour $j$ given $K$ total colour options.

> How many ways are there to colour the sides of a cube in red, green, or blue, up to rotation?

> How many ways are there to colour the sides of a cube with in red, green, or blue, if we use red three times, green twice, and blue once?

```c++
#include "core/api/Polya.hh"
#include <vector>
using namespace polya;
using namespace std;

orbits::countOrbits(groups::cube(), orbits::ColourCount{3}); // 57

evaluateColours(
    cycleIndexPolynomial(groups::cube()),
    orbits::ColourCount{3}
).coefficient(
    Polynomial::Term{
        vector{Polynomial::Exponent{3}, Polynomial::Exponent{2}, Polynomial::Exponent{1}}
    }
).asInteger(); // 3
```
