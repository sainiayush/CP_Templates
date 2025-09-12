class dsu{
    public:
    dsu()=default;
	explicit dsu(size_t n):m_parentsOrSize(n,-1){}
	int leader(int i){
		if (m_parentsOrSize[i]<0)return i;
		return(m_parentsOrSize[i]=leader(m_parentsOrSize[i]));
	}
	int merge(int a,int b){
		a=leader(a);
		b=leader(b);
		if (a!=b){
			if(-m_parentsOrSize[a]<-m_parentsOrSize[b])std::swap(a,b);
			m_parentsOrSize[a]+=m_parentsOrSize[b];
			m_parentsOrSize[b]=a;
		}
		return leader(a);
	}
	bool same(int a,int b){return(leader(a)==leader(b));}
	int size(int i){return -m_parentsOrSize[leader(i)];}
	std::vector<std::vector<int>>groups(){
	    size_t n=m_parentsOrSize.size();
	    std::vector<std::vector<int>>A(n);
	    for(int i=0;i<n;i++)A[leader(i)].emplace_back(i);
	    std::vector<std::vector<int>>res;
	    for(auto a:A)if(a.size())res.emplace_back(a);
	    return res;
	}
    private:
	std::vector<int>m_parentsOrSize;
};
