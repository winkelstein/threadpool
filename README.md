# threadpool

## Example
```
void func()
{
	std::cout << "Hello, world" << std::endl;
}

int main()
{
	skyfall::ThreadPool pool(3);
	pool.execute(func);
	pool.execute(func);
	pool.execute(func);
	pool.execute(func);
	pool.execute(func);
	pool.execute(func);
}

```
