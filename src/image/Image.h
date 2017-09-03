#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Common.h"
#include "Pixel.h"

/*
 * Slice_iter and Cslice_iter are from the following:
 * http://www.stroustrup.com/3rd_code.html
 */

// Forward declarations to allow friend declarations:
template<class T> class Slice_iter;
template<class T> bool operator==(const Slice_iter<T>&, const Slice_iter<T>&);
template<class T> bool operator!=(const Slice_iter<T>&, const Slice_iter<T>&);
template<class T> bool operator< (const Slice_iter<T>&, const Slice_iter<T>&);

template<class T> class Slice_iter {
	T* v;
	slice s;
	size_t curr;	// index of current element

	T& ref(size_t i) const {
        return v[s.start() + i * s.stride()];
    }

public:
	Slice_iter(T* vv, slice ss) :v(vv), s(ss), curr(0) { }

	Slice_iter end() const
	{
		Slice_iter t = *this;
		t.curr = s.size();	// index of last-plus-one element
		return t;
	}

	Slice_iter& operator++() { curr++; return *this; }
	Slice_iter operator++(int) { Slice_iter t = *this; curr++; return t; }

	T& operator[](size_t i) { return ref(i); }		// C style subscript
	T& operator()(size_t i) { return ref(i); }		// Fortran-style subscript
	T& operator*() { return ref(curr); }			// current element

	friend bool operator==<>(const Slice_iter& p, const Slice_iter& q);
	friend bool operator!=<>(const Slice_iter& p, const Slice_iter& q);
	friend bool operator< <>(const Slice_iter& p, const Slice_iter& q);

};

template<class T>
bool operator==(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return p.curr==q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return p.curr<q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

// forward declarations to allow friend declarations:
template<class T> class Cslice_iter;
template<class T> bool operator==(const Cslice_iter<T>&, const Cslice_iter<T>&);
template<class T> bool operator!=(const Cslice_iter<T>&, const Cslice_iter<T>&);
template<class T> bool operator< (const Cslice_iter<T>&, const Cslice_iter<T>&);

template<class T> class Cslice_iter 
{
	T* v;
	slice s;
	size_t curr; // index of current element
	const T& ref(size_t i) const { return v[s.start()+i*s.stride()]; }
public:
	Cslice_iter(T* vv, slice ss): v(vv), s(ss), curr(0){}
	Cslice_iter end() const
	{
		Cslice_iter t = *this;
		t.curr = s.size(); // index of one plus last element
		return t;
	}
	Cslice_iter& operator++() { curr++; return *this; }
	Cslice_iter operator++(int) { Cslice_iter t = *this; curr++; return t; }
	
	const T& operator[](size_t i) const { return ref(i); }
	const T& operator()(size_t i) const { return ref(i); }
	const T& operator*() const { return ref(curr); }

	friend bool operator==<>(const Cslice_iter& p, const Cslice_iter& q);
	friend bool operator!=<>(const Cslice_iter& p, const Cslice_iter& q);
	friend bool operator< <>(const Cslice_iter& p, const Cslice_iter& q);

};

template<class T>
bool operator==(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return p.curr==q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return p.curr<q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

template<class T>
class Image {
public:
    Image()
        : mWidth(0),
          mHeight(0),
          mDepth(0) {
        mPixel = new T[0];
    }

    Image(size_t width,
          size_t height)
        : mWidth(width),
          mHeight(height) {
        mPixel = new T[width * height];
    }

    Image(Image<T> &other) {
        if (other != *this) {
            mWidth  = other.mWidth;
            mHeight = other.mHeight;
            if (mPixel) {
                delete[] mPixel;
            }

            memcpy(mPixel, other.mPixel, mWidth * mHeight * sizeof(T));
        }
    }

    Image(Image<T> &&rvalue)
        : mWidth(rvalue.mWidth),
          mHeight(rvalue.mHeight) {
        if (mPixel) {
            delete[] mPixel;
        }

        mPixel = move(rvalue.mPixel);
    }

    virtual ~Image() {
        delete[] mPixel;
    }

    size_t width() const {
        return mWidth;
    }

    size_t height() const {
        return mHeight;
    }

    Slice_iter<T> operator[](size_t x) {
        return column(x);
    }

	Cslice_iter<T> operator[](size_t x) const { 
        return column(x);
    }

private:
    inline Slice_iter<T> row(size_t index)
    {
        return Slice_iter<T>(mPixel, slice(index, mWidth, mHeight));
    }

    inline Cslice_iter<T> row(size_t index) const
    {
        return Cslice_iter<T>(mPixel, slice(index, mWidth, mHeight));
    }

    inline Slice_iter<T> column(size_t index)
    {
        return Slice_iter<T>(mPixel, slice(index * mHeight, mHeight, 1));
    }

    inline Cslice_iter<T> column(size_t index) const
    {
        return Cslice_iter<T>(mPixel, slice(index * mHeight, mHeight, 1));
    }

    size_t mWidth;
    size_t mHeight;
    size_t mDepth;
    T*     mPixel;
};

#endif  // __IMAGE_H__
