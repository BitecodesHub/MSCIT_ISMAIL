#include <memory>
#include <iterator>

template<typename T>
class Vector {
private:
    struct SharedData {
        std::shared_ptr<T[]> data;
        size_t size;
        size_t capacity;
    };
    
    std::shared_ptr<SharedData> sharedData;
    
    void resize() {
        size_t new_capacity = sharedData->capacity == 0 ? 1 : sharedData->capacity * 2;
        reserve(new_capacity);
    }

public:
    Vector(){
        sharedData = std::make_shared<SharedData>();
        sharedData->data = nullptr;
        sharedData->size = 0;
        sharedData->capacity = 0;
    }

    // initializer list
    Vector(std::initializer_list<T> list) {
        sharedData = std::make_shared<SharedData>();
        sharedData->data = nullptr;
        sharedData->size = 0;
        sharedData->capacity = 0;
        for (const T& value : list) {
            push_back(value);
        }
    }

    
    size_t getSize() const {
        return sharedData->size;
    }

    size_t getCapacity() const {
        return sharedData->capacity;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > sharedData->capacity) {
            std::shared_ptr<T[]> new_data(new T[new_capacity], std::default_delete<T[]>());
            for (size_t i = 0; i < sharedData->size; ++i) {
                new_data[i] = sharedData->data[i];
            }
            sharedData->data = new_data;
            sharedData->capacity = new_capacity;
        }
    }

    void push_back(const T& value) {
        if (sharedData->size == sharedData->capacity) {
            resize();
        }
        sharedData->data[sharedData->size] = value;
        sharedData->size++;
    }

    void pop_back() {
        if (sharedData->size == 0) {
            throw std::out_of_range("Cannot pop_back from an empty vector");
        }
        sharedData->size--;
    }

    void insert(size_t index, const T& value) {
        if (index > sharedData->size) {
            throw std::out_of_range("Index out of range");
        }

        if (sharedData->size == sharedData->capacity) {
            resize();
        }

        for (size_t i = sharedData->size; i > index; i--) {
            sharedData->data[i] = sharedData->data[i - 1];
        }

        sharedData->data[index] = value;
        sharedData->size++;
    }

    void erase(size_t index) {
        if (index >= sharedData->size) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = index; i < sharedData->size - 1; i++) {
            sharedData->data[i] = sharedData->data[i + 1];
        }
        sharedData->size--;
    }

    void clear() {
        sharedData->size = 0;
    }

    T& operator[](size_t index) {
        if (index >= sharedData->size) {
            throw std::out_of_range("Index out of range");
        }
        return sharedData->data[index];
    }

    // Enable the copy constructor
    Vector(const Vector& other)
        : sharedData(other.sharedData) {}

    // Enable the assignment operator
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            sharedData = other.sharedData;
        }
        return *this;
    }

    class Iterator {

        public:
        //using value_type = T;
        //using difference_type = std::ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        pointer ptr;

        Iterator(pointer ptr){
            this->ptr = ptr;
        }

        bool operator!=(Iterator other){
            return ptr != other.ptr;
        }

		bool operator==(Iterator other){
			return ptr == other.ptr;
		}

        reference operator*(){
            return *ptr;
        }

        void operator++(){
            ++ptr;
        }

		Iterator operator--(){
			ptr--;
			return *this;
		}

		difference_type operator-(Iterator other) const {
			return ptr - other.ptr;
		}

		Iterator operator-(difference_type n){
			return Iterator(ptr - n);
		}

		Iterator operator+(difference_type n){
			return Iterator(ptr + n);
		}
		bool operator<(Iterator other){
			return ptr < other.ptr;
		}
    };


    Iterator begin(){
        // Method that extracts the raw pointer from the smart pointer without transferring ownership
        return Iterator(this->sharedData->data.get());
    }

    Iterator end(){
        return Iterator(sharedData->data.get() + sharedData->size);
    }

};
