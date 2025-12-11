#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <utility>
#include <string>

namespace py = pybind11;

class PriorityQueue {
private:
    std::vector<std::pair<float, std::string>> heap;

    void sift_up(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;

            if (heap[index].first <= heap[parent].first) {
                break;
            }

            std::swap(heap[index], heap[parent]);

            index = parent;
        }
    }

    void sift_down(std::size_t index) {
        while (true) {
            std::size_t size = heap.size();
            std::size_t largest = index;
            std::size_t left = 2 * index + 1;
            std::size_t right = 2 * index + 2;

            if (left < size && heap[left].first > heap[largest].first) {
                largest = left;
            }

            if (right < size && heap[right].first > heap[largest].first) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue() = default;

    void push(float danger, const std::string& name) {
        // emplace_back = shortcut version of push_back that automatically calls std::make_pair 
        heap.emplace_back(danger, name);
        heap.sift_up(heap.size()-1);

        // Old Code
        // std::size_t i = 0;
        // while (i < priority.size() && priority[i].first >= danger) {
        //     ++i;
        // }
        // priority.insert(priority.begin() + i, std::make_pair(danger, name));
    }

    std::string pop() {
        if (heap.empty()) {
            return "None!";
        }

        auto rootValue = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
    
        if (!heap.empty()) {
            heap.sift_down(0);
        }
    
        return rootValue.second();

        // Old Code
        // auto val = priority.front();
        // priority.erase(priority.begin());
        // return val.second;
    }

    float top() const {
        if (heap.empty()) {
            return 0;
        }
        return static_cast<int>(heap.front().first);
    }

    bool is_empty() const {
        return heap.empty();
    }

    std::size_t size() const {
        return static_cast<int>(heap.size());
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
