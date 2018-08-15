#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T>
void draw(const T &x, std::ostream &out, size_t position) {
    out << std::string(position, ' ') << x << std::endl;
}


class object_t
{   // polymorphic object that holds anything implementing a
    // draw function
 public:
    template <typename T> // templated constructor, T models drawable
    object_t(T x) : self_(std::make_shared<model<T>>(std::move(x))) {
        // pass sink argument by value and move them into place
    }

    friend void draw(const object_t &x, std::ostream &out, size_t position) {
        x.self_->draw_(out, position);
    }

 private: // nested private interface
    struct concept_t
    {
        virtual ~concept_t() = default;
        virtual void draw_(std::ostream &, size_t) const = 0;
    };

    template <typename T>
    struct model : concept_t
    {
        model(T x) : data_(std::move(x)) {}
        virtual void draw_(std::ostream &out, size_t position) const override {
            draw(data_, out, position);
        }
        T data_;
    }; // a shared ptr to an immutable (const) object has value semantics
    std::shared_ptr<const concept_t> self_;
};





using document_t = std::vector<object_t>;

void draw(const document_t &x, std::ostream &out, size_t position) {
    out << std::string(position, ' ') << "<document>" << std::endl;
    for (auto &e : x)
        draw(e, out, position + 2);
    out << std::string(position, ' ') << "<document>" << std::endl;
}

using history_t = std::vector<document_t>;

void commit(history_t &x) {
    assert(x.size());
    x.push_back(x.back());
}
void undo(history_t &x) {
    assert(x.size());
    x.pop_back();
}
document_t &current(history_t &x) {
    assert(x.size());
    return x.back();
}

/******************************************************************************/
// Client
class my_class_t
{
    /* public: */
    /*  void draw(std::ostream& out, size_t position) const { */
    /*    out << std::string(position, ' ') << "my_class_t" << std::endl; */
    /*  } */
};

void draw(const my_class_t &, std::ostream &out, size_t position) {
    out << std::string(position, ' ') << "my_class_t" << std::endl;
}

int main() {
    history_t h(1);

    current(h).emplace_back(0);
    current(h).emplace_back(std::string("Hello!"));

    draw(current(h), std::cout, 0);
    std::cout << "--------------------------" << std::endl;

    commit(h);

    current(h).emplace_back(current(h));
    current(h).emplace_back(my_class_t());
    current(h)[1] = std::string("World");

    draw(current(h), std::cout, 0);
    std::cout << "--------------------------" << std::endl;

    undo(h);

    draw(current(h), std::cout, 0);
}
