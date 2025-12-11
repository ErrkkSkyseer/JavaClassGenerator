public abstract class Sample extends Parent implements ISampleInterface1, ISampleInterface2 
{
    public Type publicField;

    protected Type protectedField;

    private Type privateField1;
    private Type privateField2;

    public Sample(P param1, P param2) 
    {

    }

    public Type getPublicField() {return publicField};
    
    public void setPublicField(Type value) {publicField = value};

    public Type publicFunction(P param1, P param2)
    {
        return null;
    }

    protected Type protectedFunction(P param1, P param2)
    {
        return null
    }

    private Type privateFunction1(P param1, P param2)
    {
        return null;
    }
    private void privateFunction2()
    {
        
    }
}