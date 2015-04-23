// ... comments not include  ...
///  A example of class : MyClass. 
/**
    a more detail class description :
    \author Me
    \date 29/07/2005
*/
#include <string>
class MyClass
{


    /* ... comments not include ... */
    public:
        /** the constructor of the class */

        /**  the detail description of the constructor. */
        MyClass(){i=0;}
 
        //! A destructor.
        ~MyClass(){}

        /// drawing of a string
        /**
            \param s the string to display
            \return there is no return
            @sa MyClass(), ~MyClass()
        */
        void draw(const char *s="Hello World");

        /* exemple of doc comments not before the declaration */
        unsigned int getI(){return i;} /**<@return the number 
                                                the value of i */
    private:
        unsigned int i;


    /** @var i
        @brief, you can put the comments where you want 
                                    with the special tags */
};
