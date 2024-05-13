// homework_mo_3.2.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Вторая задаче из третьей лекции модуля Многопоточность.

//оператор foreach

//Реализуйте параллельный вариант оператора for_each.
//Функция должна принимать итераторы на начало и конец контейнера и ссылку, обрабатывающую функцию.
//При реализации нужно рекурсивно разделить контейнер на несколько блоков и для каждого запустить отдельную задачу, применяющую обрабатывающую функцию к части контейнера.

#include <iostream>
#include <future>
#include <algorithm>
#include <array>
#include <numeric>
#include <vector>
/*
template<typename T1, typename T2>
T2 para_foreach(T1 begin, T1 end, T2(*ff)(T2))
{
    int size = 1;
    // считаем длину интервала, в данном случае должно получиться шесть

    auto curr_size = std::distance(begin, end);
    // если наш интервал равен единичке, то есть он самый маленький, то просто возвращаем число, на которое показывает итератор begin плюс десятка.

    if (curr_size <= size)
    {
        T2 para_foreach(T1 begin, T1 end, T2 &&ff);
    }
    // если иначе, то есть интервал больше единицы, то делим его пополам
    auto mid = begin;

    // мы ходим получить здесь итератор, который показывает середину интервала
    // функция advance сдвинула наш итератор вперёд, на середину оставшегося интервала.
    // сейчас он на начало показывает, а чтобы он показал на середину, нужно текущий интервал разделить пополам
    std::advance(mid, curr_size / 2);

    // левую часть интервала будем считать в асинхронной задаче ft_res
    // левая часть от начала до середины.
    auto ft_res = std::async(para_foreach<T1,T2>, begin, mid, ff);
    // для левой части вызываем рекурсивно функцию от середины до конца
    auto lf_res = para_foreach(mid, end, ff);

    // выходим из рекурсии, вызвав результат будущего
    return ft_res.get();
}

int Fn(int a)
{
    std::cout << a + 10 << std::endl;
    return a + 10;
}
*/

template <typename It, typename Fn>
void parallel_for_each(It begin, It end, Fn&& function)
{
    const size_t block_size = 4;
    size_t diff = static_cast<size_t>(std::distance(begin, end));

    if (diff > 0)
    {
        size_t size = diff;

        if (diff > block_size)
        {
            size = size / 2;
            It endAs = begin + size;
            std::future<void> q = std::async(std::launch::async, [begin, endAs, function]
                { parallel_for_each(begin, endAs, function); });

            q.wait();
            parallel_for_each(endAs, end, function);
        }
        else
        {
            std::for_each(begin, end, function);
        }
    }
}

int Fn(int a)
{
    std::cout << a + 10 << std::endl;
    return a + 10;
}

int main()
{
    std::vector<int> V = { 0, 1, 2, 3 };
    parallel_for_each(V.begin(), V.end(), Fn);

    return 0;
}
