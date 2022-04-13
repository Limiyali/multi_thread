#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
using namespace std;

template <class T>
class MyQueue
{
public:
    MyQueue() = default;
    MyQueue &operator=(const MyQueue &) = delete;
    void push(T x)
    {
        auto new_node = make_unique<Node>(move(x));
        Node *new_node_tail = new_node.get();
        if (tail_)
            tail_->next = move(new_node);
        else
            head_ = move(new_node);
        tail_ = new_node_tail;
    }
    shared_ptr<T> try_pop()
    {
        if (!head_)
            return nullptr;
        auto res = make_shared<T>(move(head_->val));
        unique_ptr<Node> head_node = move(head_);
        head_ = move(head_node->next);
        return res;
    }

private:
    struct Node
    {
        explicit Node(T x) : val(move(x)) {}
        T val;
        unique_ptr<Node> next;
    };
    unique_ptr<Node> head_;
    Node *tail_ = nullptr;
};

template <class T>
class ConcurrentQueue
{
public:
    ConcurrentQueue():head_(new Node),tail_(head_.get()){};
    ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;
    void push(T x)
    {
        auto new_val = make_shared<T>(move(x));
        auto new_node = make_unique<Node>();
        Node *new_node_tail = new_node.get();

        lock_guard<mutex> lk(tail_mutex_);
        tail_->val = new_val;
        tail_->next = move(new_node);
        tail_ = new_node_tail;
    }
    shared_ptr<T> try_pop()
    {
        auto head_node = pop_heap();
        return head_node ? head_node->val : nullptr;
    }

private:
    struct Node
    {
        shared_ptr<T> val;
        unique_ptr<Node> next;
    };

private:
    unique_ptr<Node> pop_heap()
    {
        lock_guard<mutex> l(head_mutex_);
        if (head_.get() == get_tail())
            return nullptr;
        unique_ptr<Node> head_node = move(head_);
        head_ = move(head_node->next);
        return head_node;
    }
    Node *get_tail()
    {
        lock_guard<mutex> l(tail_mutex_);
        return tail_;
    }
    unique_ptr<Node> head_;
    Node *tail_ = nullptr;
    mutex head_mutex_;
    mutex tail_mutex_;
};

int main()
{
    ConcurrentQueue<int> a;
    a.push(1);
    a.push(3);
    cout << "##############\n";
    cout << *a.try_pop() << endl;
    cout << *a.try_pop()<<endl;
}