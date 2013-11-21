/*
Parent class
Children mult implement takedown
Provides opengl return error checking
*/
#pragma once
#include "include.h"


class BasicGL
{
    public:
        BasicGL() {}
        //Children must implement this function
        virtual void TakeDown() = 0;

    protected:

        //Check for opengl Errors and output if error along with input char strings
        bool GLReturnedError(const char * s) {
            bool return_error = false;
            GLenum glerror;
            //Go through list of errors until no errors remain
            while ((glerror = glGetError()) != GL_NO_ERROR) {
                return_error = true;
                std::cerr << s << ": " << gluErrorString(glerror) << std::endl;
            }

            return return_error;
        }
};
