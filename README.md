# Hash Number Optimizer/Searcher
Program to search for better hash functions. See why do this section

We have a hash funcition with unknown coefficients

```c++

size_t hash(const Point2D& k, int x, int y) 
{
	unsigned long int a = k.x * x;
	unsigned long int b = k.y * y + std::numeric_limits<int>::max();
	if (k.x <= 0) {
		a = a << b;
	}
	if (k.y <= 0)
	{
		b = b << a <<1;
	}

	size_t res = a ^ b;
	
	return res;
}

```

We want to find a most optimal - with the least collisions on 40k dataset (points in (x, y) range -1000, 1000).
Current state should be optimized, it's just brute-force, for each point generate 40000 hashes and find how much collide.
I left points (1, y) and (x, 1) unchecked as nothing special seems to be there.
Current best is
(5, 941) - collision rate 12.0675%
Checked from (2, 2) to (89, 743) (87569 runs)
The code needs to be optimized somehow as now it takes a horrendeus amount of time to work, at least on my computer

# Why do this
I was searching for a simple and good hash-function. The first approach was from StackOverflow: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
The version for my class
```c++
size_t operator()( const Key& k ) const
{
    // Compute individual hash values for first, second and third
    // http://stackoverflow.com/a/1646913/126995
    size_t res = 17;
    res = res * 31 + hash<string>()( k.first );
    res = res * 31 + hash<string>()( k.second );
    res = res * 31 + hash<int>()( k.third );
    return res;
}
```

But the tested class the hash was made for made it ultimately unefficient

```c++
struct Point2D
{
	int x;
	int y;

	Point2D() = delete;

	Point2D(int _x, int _y) {
		x = _x;
		y = _y;
	}

	bool operator==(const Point2D& other) const
	{
		return (x == other.x && y == other.y);
	}
};
```

There were a lot a lot alot of collisionn (more than 98% on 400000 runs - the testing data are points in range -1000, 1000).

Therefore the search for something better began.

The first try was to simply move some values forward

```c++
{
  unsigned long int a = k.x;
  unsigned long int b = k.y + std::numeric_limits<int>::max();
  if (k.x <= 0) {
    a = a >> 1;
  }
  if (k.y <= 0)
  {
    b = b >> 1;
  }
  size_t res = a ^ b;

  return res;
}
 ```
 
 It was a bit better, the collision rate dropped to 90% but it was still to high. Then I was just experimenting without anything in mind.
 
 ```c++
 size_t hash(const Point2D& k) 
{
	unsigned long int a = k.x ;
	unsigned long int b = k.y  + std::numeric_limits<int>::max();
	if (k.x <= 0) {
		a = a << b;
	}
	if (k.y <= 0)
	{
		b = b << a <<1;
	}

	size_t res = a ^ b;
	
	return res;
}
```

Made the collision rate drop to 55% which was in fact a surprise. Then a question raised - why don't just multiplicate with random values

```c++
size_t hash(const Point2D& k) 
{
	unsigned long int a = k.x * 567;
	unsigned long int b = k.y * 321 + std::numeric_limits<int>::max();
	if (k.x <= 0) {
		a = a << b;
	}
	if (k.y <= 0)
	{
		b = b << a <<1;
	}

	size_t res = a ^ b;
	
	return res;
}
```

And this was quite a surprise as the collision rate dropped to 30%. Then the search for best coeficients began.

