#include <utility>

typedef double               real;
typedef std::pair<real,real> loc;
struct cloc { real first; real second; };

struct Shape;
struct Circle;
struct Square;
struct Triangle;

struct ShapeVisitor
{
	virtual void visit(const Circle&)   {}
	virtual void visit(const Square&)   {}
	virtual void visit(const Triangle&) {}
};

// An Algebraic Data Type implemented through inheritance

struct Shape
{
    virtual void accept(ShapeVisitor& v) = 0;
};

struct Circle : Shape
{
    Circle(const loc& c, const real& r) : center(c), radius(r) {}

    void accept(ShapeVisitor& v) { v.visit(*this); }

    const loc& get_center() const { return center; }

    loc  center;
    real radius;
};


struct Square : Shape
{
    Square(const loc& c, const real& s) : upper_left(c), side(s) {}

    void accept(ShapeVisitor& v) { v.visit(*this); }

    loc  upper_left;
    real side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}

    void accept(ShapeVisitor& v) { v.visit(*this); }

    loc first;
    loc second;
    loc third;
};

// An Algebraic Data Type implemented as discriminated union

struct ADTShape
{
	enum shape_kind {circle, square, triangle} kind;

#ifndef POD_ONLY
	ADTShape(const cloc& c, real r) : kind(circle), center(c), radius(r) {}
	ADTShape(real s, const cloc& c) : kind(square), upper_left(c), size(s) {}
	ADTShape(const cloc& x, const cloc& y, const cloc& z) : kind(triangle), first(x), second(y), third(z) {}
	virtual ~ADTShape() {}
#endif

#ifndef _MSC_VER
	union
	{
		struct { cloc center;     real radius; }; // as_circle;
		struct { cloc upper_left; real size; }; //   as_square;
		struct { cloc first, second, third; }; //    as_triangle;
	};
#else
    // MSVC doesn't seem to allow anonymous structs inside union so we just dump
    // here same members name directly to make the rest just compile at least.
	cloc center;     real radius; // as_circle;
	cloc upper_left; real size;   // as_square;
	cloc first, second, third;    // as_triangle;
#endif
};

// A slight extension of the above with additional field

struct ADTShapeEx : ADTShape
{
	ADTShapeEx(const ADTShape& s) : ADTShape(s) {}
	int extra_field;
};

//real Triangle::*m = &Triangle::second.first;
