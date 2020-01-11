#ifndef UNTITLE06_STACK_H
#define UNTITLE06_STACK_H


#include <iterator>
#include <iostream>
#include <memory> //for smart ptr


template<class T>
struct Stack {
private:
    struct Stack_node;

public:
    size_t Size = 0;
    Stack() = default;
    //Stack(const Stack &) = delete; // для копирования

    struct forward_iterator {
        using value_type = T; // using делается для совместимости со станд алг-ми STL
        using reference = T &;
        using pointer = T *;
        using difference_type = std::ptrdiff_t; //значения смещений итераторов относительно друг друга
        using iterator_category = std::forward_iterator_tag;

        forward_iterator(Stack_node *ptr);

        T &operator*();//разименование
        forward_iterator &operator++(); // ++it
        forward_iterator operator++(int); //it++

        bool operator==(const forward_iterator &o) const;

        bool operator!=(const forward_iterator &o) const;// писат конст желательно для конст объектов и результатов

    private:
        Stack_node *ptr_; //итератор должен иметь указатель на узел стэка

        friend struct Stack;
    };

    forward_iterator begin();

    forward_iterator end();

    T &Top();

    void insert(const forward_iterator &it, const T &value);//вставка
    void erase(const forward_iterator &it); //удаление
    void Push(const T &value);

    void Pop();

private:
    struct Stack_node {
        T value_; //потом подправить под фигуру
        std::unique_ptr<Stack_node> next_ = nullptr;

        forward_iterator next();

        Stack_node() = default;

        Stack_node(const T &value, std::unique_ptr<Stack_node> next) : value_(value), next_(std::move(next)) {};
    };


    std::unique_ptr<Stack_node> head_ = nullptr;
};

template<class T>
typename Stack<T>::forward_iterator Stack<T>::Stack_node::next() {
    return { next_.get()};
}

template<class T>
typename Stack<T>::forward_iterator Stack<T>::begin() {
    if (head_ == nullptr) {
        return nullptr;
    }
    return head_.get();
}

//typename используем, чтобы компилятор понял, что это
template<class T>
typename Stack<T>::forward_iterator Stack<T>::end() {
    return nullptr;
}


template<class T>
Stack<T>::forward_iterator::forward_iterator(Stack_node *ptr): ptr_(ptr) {}

template<class T>
T &Stack<T>::forward_iterator::operator*() {
    return ptr_->value_;
}

template<class T>
typename Stack<T>::forward_iterator &Stack<T>::forward_iterator::operator++() {
    *this = ptr_->next();
    return *this;
}

template<class T>
typename Stack<T>::forward_iterator Stack<T>::forward_iterator::operator++(int) {
    forward_iterator old = *this;
    ++*this;
    return old;
}

template<class T>
bool Stack<T>::forward_iterator::operator==(const forward_iterator &o) const {
    return ptr_ == o.ptr_;
}

template<class T>
bool Stack<T>::forward_iterator::operator!=(const forward_iterator &o) const {
    return ptr_ != o.ptr_;
}


template<class T>
void Stack<T>::insert(const forward_iterator &it, const T &value) {
    std::unique_ptr<Stack_node> new_node(new Stack_node(value, nullptr));

    if (it.ptr_ == nullptr && Size != 0) {
        throw std::logic_error("iterator went beyond the bounds of the stack");
    } else if (it.ptr_ == nullptr && Size == 0) {
        head_ = std::unique_ptr<Stack_node>(new Stack_node(value, nullptr));
        ++Size;
    } else {
        new_node->next_ = std::move(it.ptr_->next_);
        it.ptr_->next_ = std::move(new_node);
        ++Size;
    }
}

template<class T>
void Stack<T>::erase(const typename Stack<T>::forward_iterator &it) {
    if (it.ptr_ == nullptr) {
        throw std::logic_error("erasing invalid iterator");
    } else if (it == this->begin()) {
        head_ = std::move(it.ptr_->next_);
        --Size;
    } else {
        auto it2 = this->begin(); // поиск элемента перед элементом, на который указывает итератор
        while (it2.ptr_->next() != it.ptr_) {
            ++it2;
        }
        it2.ptr_->next_ = std::move(it.ptr_->next_);
    }
}

template<class T>
void Stack<T>::Push(const T &value) {
    std::unique_ptr<Stack_node> new_node(new Stack_node(value, nullptr));
    new_node->next_ = std::move(head_);
    head_ = std::move(new_node);
    ++Size;
}

template<class T>
T &Stack<T>::Top() {
    if (head_.get()) {
        return head_->value_;
    } else {
        throw std::logic_error("Stack is empty");
    }
}

template<class T>
void Stack<T>::Pop() {
    if (head_ == nullptr) {
        throw std::logic_error("Stack is empty");
    } else {
        head_ = std::move(head_->next_);
        --Size;
    }
}


#endif //UNTITLE06_STACK_H
