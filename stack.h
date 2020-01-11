#ifndef UNTITLE06_STACK_H
#define UNTITLE06_STACK_H

#include <iterator>
#include <memory>


template<class T, class Allocator = std::allocator<T>>
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
    using allocator_type = typename Allocator::template rebind<Stack_node>::other;

    struct deleter {
        deleter(allocator_type* allocator): allocator_(allocator) {}

        void operator() (Stack_node* ptr) {
            if(ptr != nullptr){
                std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                allocator_->deallocate(ptr, 1);
            }
        }

    private:
        allocator_type* allocator_;
    };

    using unique_ptr = std::unique_ptr<Stack_node, deleter>;

    struct Stack_node {
        T value_;
        unique_ptr next_ {nullptr, deleter{&this->allocator_}};

        forward_iterator next();

        Stack_node() = default;

        Stack_node(const T &value, unique_ptr next) : value_(value), next_(std::move(next)) {};
    };

    unique_ptr head_ {nullptr, deleter{&this->allocator_}};

    allocator_type allocator_{}; //конструктор дефолтный для поля
};

template<class T, class Allocator>
typename Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::Stack_node::next() {
    return { next_.get()};
}

template<class T, class Allocator>
typename Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::begin() {
    if (head_ == nullptr) {
        return nullptr;
    }
    return head_.get();
}

//typename используем, чтобы компилятор понял, что это
template<class T, class Allocator>
typename Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::end() {
    return nullptr;
}


template<class T, class Allocator>
Stack<T, Allocator>::forward_iterator::forward_iterator(Stack_node *ptr): ptr_(ptr) {}

template<class T, class Allocator>
T &Stack<T, Allocator>::forward_iterator::operator*() {
    return ptr_->value_;
}

template<class T, class Allocator>
typename Stack<T, Allocator>::forward_iterator &Stack<T, Allocator>::forward_iterator::operator++() {
    *this = ptr_->next();
    return *this;
}

template<class T, class Allocator>
typename Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::forward_iterator::operator++(int) {
    forward_iterator old = *this;
    ++*this;
    return old;
}

template<class T, class Allocator>
bool Stack<T, Allocator>::forward_iterator::operator==(const forward_iterator &o) const {
    return ptr_ == o.ptr_;
}

template<class T, class Allocator>
bool Stack<T, Allocator>::forward_iterator::operator!=(const forward_iterator &o) const {
    return ptr_ != o.ptr_;
}


template<class T, class Allocator>
void Stack<T, Allocator>::insert(const forward_iterator &it,const T &value) {
    Stack_node* newptr = this->allocator_.allocate(1);
    std::allocator_traits<allocator_type>::construct(this->allocator_, newptr, value,
            std::unique_ptr<Stack_node, deleter>(nullptr, deleter{&this->allocator_}));
    unique_ptr new_node(newptr, deleter{&this->allocator_});

    if (it.ptr_ == nullptr && Size != 0) {
        throw std::logic_error("iterator went beyond the bounds of the stack");
    } else if (it.ptr_ == nullptr && Size == 0) {
        new_node->next_ = std::move(head_);
        head_ = std::move(new_node);
        ++Size;
    } else {
        new_node->next_ = std::move(it.ptr_->next_);
        it.ptr_->next_ = std::move(new_node);
        ++Size;
    }
}

template<class T, class Allocator>
void Stack<T, Allocator>::erase(const typename Stack<T, Allocator>::forward_iterator &it) {
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

template<class T, class Allocator>
void Stack<T, Allocator>::Push(const T &value) {
    Stack_node* newptr = this->allocator_.allocate(1); // получили ссылку на память, в которую можно положить элемент
    std::allocator_traits<allocator_type>::construct(this->allocator_, newptr, value, std::unique_ptr<Stack_node, deleter>(
            nullptr, deleter{&this->allocator_})); // к
    unique_ptr new_node(newptr, deleter{&this->allocator_});
    new_node->next_ = std::move(head_);
    head_ = std::move(new_node);
    ++Size;
}

template<class T, class Allocator>
T &Stack<T, Allocator>::Top() {
    if (head_.get()) {
        return head_->value_;
    } else {
        throw std::logic_error("Stack is empty");
    }
}

template<class T, class Allocator>
void Stack<T, Allocator>::Pop() {
    if (head_ == nullptr) {
        throw std::logic_error("Stack is empty");
    } else {
        head_ = std::move(head_->next_);
        --Size;
    }
}


#endif //UNTITLE06_STACK_H
