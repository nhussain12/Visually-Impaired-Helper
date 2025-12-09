#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <utility>
#include <string>

namespace py = pybind11;

class PriorityQueue {
private:
    std::vector<std::pair<float, std::string>> priority;

public:
    PriorityQueue() = default;

    void push(float danger, const std::string& name) {
        std::size_t i = 0;
        while (i < priority.size() && priority[i].first >= danger) {
            ++i;
        }
        priority.insert(priority.begin() + i, std::make_pair(danger, name));
    }

    std::string pop() {
        if (priority.empty()) {
            return "None!";
        }
        auto val = priority.front();
        priority.erase(priority.begin());
        return val.second;
    }

    float top() const {
        if (priority.empty()) {
            return 0;
        }
        return static_cast<int>(priority.front().first);
    }

    bool is_empty() const {
        return priority.empty();
    }

    std::size_t size() const {
        return static_cast<int>(priority.size());
    }
};

PYBIND11_MODULE(bindings, m) {
    m.doc() = "Priority Queue for the visually impaired :)";

    py::class_<PriorityQueue>(m, "PriorityQueue")
        .def(py::init<>())
        .def("push", &PriorityQueue::push)
        .def("pop", &PriorityQueue::pop)
        .def("top", &PriorityQueue::top)
        .def("is_empty", &PriorityQueue::is_empty)
        .def("size", &PriorityQueue::size);
}
