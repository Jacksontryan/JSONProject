//
// Created by jacks on 4/27/2026.
//

#ifndef VIDEOGAME_COLLECTION_H
#define VIDEOGAME_COLLECTION_H

template <class T>
class Collection {
    protected:
    int length;
    public:
    int size() {
        return this->length;
    }
};


#endif //VIDEOGAME_COLLECTION_H