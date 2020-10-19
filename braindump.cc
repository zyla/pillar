#include <iostream>

using Datum = std::variant<int64_t, std::string>;

enum CompareOp { EQ };

class DynamicColumn {
    virtual ~DynamicColumn() = 0;
    virtual std::vector<Index> sort();
    virtual Bitmap threshold(CompareOp op, const Datum &value);
    virtual std::unique_ptr<DynamicColumn> filter(const Bitmap &) const;
    
    virtual std::vector<std::tuple<Index, Index>> eq_join(const DynamicColumn &);
};

class Bitmap {
    std::vector<Index> to_indices();
    
    void set(int, bool);
    
    // Number of ones
    int popcount();
    
    Bitmap and(const Bitmap &b) const;
    Bitmap or(const Bitmap &b) const;
};

template<typename T>
class Column : public DynamicColumn {
    void sort(std::vector<Index> &indices) override {
        std::stable_sort(std::begin(indices), std::end(indices), [&x](int a, int b) { return data[a] < data[b]; });
    }
    
    override Bitmap threshold(CompareOp op, const Datum &generic_value) {
        const T value = std::get<T>(generic_value); //  catch (std::bad_variant_access&)
        Bitmap b(this->size());
        if(op == EQ) {
            for(int i = 0; i < this->size(); i++) {
                b.set(i, this->data[i] == value);
            }
        } else {
            throw "unsupported operation xd";
        }
    }
    
    std::unique_ptr<DynamicColumn> filter(const Bitmap &b) const {
        std::vector<T> result;
    }
    

    std::vector<std::tuple<Index, Index>> eq_join(const DynamicColumn &column) override {
        auto& col = dynamic_cast<Column<T>&>(column);
    }
    
private:
    std::vector<T> data;
};
