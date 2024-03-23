#ifndef IBINDABLE_MODEL_H
#define IBINDABLE_MODEL_H

// Specifies an object that is bindable to URL encoded Http Post data.
class IBindableModel
{
public:
    // Method is invoked when model binding is initiated.
    virtual void onBindInit() {};

    // Model specific implementation for binding to http post data.
    virtual void onBind(const char *formKey, const char *formVal) = 0;

    // Method is invoked when model binding is complete.
    virtual void onBindComplete() {};
};

#endif