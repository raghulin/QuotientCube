#ifndef QUOTIENTCUBE_H
#define QUOTIENTCUBE_H


class QuotientCube
{
public:
    /** Default constructor */
    QuotientCube();
    /** Default destructor */
    virtual ~QuotientCube();
    /** Copy constructor
     *  \param other Object to copy from
     */
    QuotientCube(const QuotientCube& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    QuotientCube& operator=(const QuotientCube& other);
protected:
private:
};

#endif // QUOTIENTCUBE_H
