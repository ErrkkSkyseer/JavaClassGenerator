public abstract class Sample extends Parent implements ISampleInterface1, ISampleInterface2
{
	public Type publicField;

	protected Type protectedField;

	private Type m_privateField1;
	private Type m_privateField2;

	public Type getPublicField()
	{
		return m_publicField;
	}

	public void setPublicField(Type value)
	{
		m_publicField = value;
	}

	public Sample(P param1, P param2)
	{
		
	}

	public Type publicFunction(P param1, P param2)
	{
		return null;
	}

	protected Type protectedFunction(P param1, P param2)
	{
		return null;
	}

	private Type privateFunction2(P param1, P param2)
	{
		return null;
	}

	private void privateFunction2()
	{
		
	}

}
