// based on paper: https://www.cs.princeton.edu/courses/archive/spring12/cos423/bib/FractionalCascading1.pdf

class CatalogRecord{
    private:
        CatalogRecord* upPointer;
        int key;
    public:
        CatalogRecord(int key){
            this->key = key;
            this->upPointer = nullptr;
        }

        CatalogRecord(int key, CatalogRecord* upPointer){
            this->key = key;
            this->upPointer = upPointer;
        }

        void setUpPointer(CatalogRecord* upPointer){
            this->upPointer = upPointer;
        }

        int getKey(){
            return key;
        }

        CatalogRecord* getUpPointer(){
            return upPointer;
        }
};

