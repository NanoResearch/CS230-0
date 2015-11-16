

template <class Type>
class mein{

private:
	Type i;

protected:

public:
	mein(Type value){
		this->i = value;
	}

	virtual ~mein(){

	}

	Type get(){
		return this->i;
	}


};
