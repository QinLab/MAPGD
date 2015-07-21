#include "locus.h"

/*! \brief Locus is initialized to id0=0, id1=0, and set to contain 0 samples.
*/	
Locus::Locus(void){
	sample.clear();
	id0=0;
	id1=0;	
};
/*! \breif Locus has a memory leak, but I don't know where it is comming from, so I've
 *	defined the crap out of it. Still leaking though.
 */

/*! \brief Locus is initialized to id0=0, id1=0, and set to contain 0 samples.
*/	
Locus::Locus(count_t size){
	sample.assign(size, quartet() );
	id0=0;
	id1=0;	
}

Locus & Locus::operator =(const Locus& rhs){
        sample=rhs.sample; 	
        id0=rhs.id0;
	id1=rhs.id1;  
        extraid=rhs.extraid;   
	return *this;
}

Locus & Locus::operator+=(const Locus &rhs) {
	if (rhs.id0==this->id0 && rhs.id1==this->id1 ){
		sample.reserve( sample.size() + rhs.sample.size() ); // preallocate memory
		sample.insert( sample.end(), rhs.sample.begin(), rhs.sample.end() );
		return *this;
	}
	std::cerr << "operator undefined for different loci.\n";
	return *this;
}

const Locus Locus::operator +(const Locus& rhs) const {
	return Locus (*this) += rhs;
}

void Locus::unmask(quartet_t *q){
	q->masked=false;
}

void Locus::mask(quartet_t *q){
	q->masked=true;
}

void Locus::unmask(count_t a){
	if(a<sample.size() ) sample[a].masked=false;
}

void Locus::unmaskall(void){
	for (unsigned int s=0; s<sample.size();++s){
		sample[s].masked=false;
	}
}

void Locus::maskall(void){
	for (unsigned int s=0; s<sample.size();++s){
		sample[s].masked=true;
	};
};

count_t Locus::maskedcount(void) const
{
	count_t count=0;
	for(std::vector<quartet>::const_iterator it = sample.begin(); it != sample.end(); ++it) if (it->masked) count++;
	return count;
};

void Locus::sort(void)
{
	count_t total[5]={0};
	
	for (unsigned int s=0; s<sample.size();++s){
		if (sample[s].masked) continue;
		total[0]+=sample[s].base[0];
		total[1]+=sample[s].base[1];
		total[2]+=sample[s].base[2];
		total[3]+=sample[s].base[3];
		total[4]+=sample[s].base[4];
	};
	if (total[sorted_[0]]<total[sorted_[2]])
		std::swap(sorted_[0], sorted_[2]);
	if (total[sorted_[1]]<total[sorted_[3]])
		std::swap(sorted_[1], sorted_[3]);
	if (total[sorted_[2]]<total[sorted_[3]])
		std::swap(sorted_[2], sorted_[3]);
	if (total[sorted_[0]]<total[sorted_[1]])
		std::swap(sorted_[0], sorted_[1]);
	if (total[sorted_[1]]<total[sorted_[2]])
		std::swap(sorted_[1], sorted_[2]);
}

void Locus::sort(count_t s)
{
	if (sample[s].base[sorted_[0]]<sample[s].base[sorted_[2]])
		std::swap(sorted_[0], sorted_[2]);
	if (sample[s].base[sorted_[1]]<sample[s].base[sorted_[3]])
		std::swap(sorted_[1], sorted_[3]);
	if (sample[s].base[sorted_[2]]<sample[s].base[sorted_[3]])
		std::swap(sorted_[2], sorted_[3]);
	if (sample[s].base[sorted_[0]]<sample[s].base[sorted_[1]])
		std::swap(sorted_[0], sorted_[1]);
	if (sample[s].base[sorted_[1]]<sample[s].base[sorted_[2]])
		std::swap(sorted_[1], sorted_[2]);
}

void Locus::swap(count_t x, count_t y)
{	
	std::swap(sorted_[x], sorted_[y]);
}

count_t Locus::getcount(count_t s, count_t c) const
{
	if (c<5) return sample[s].base[sorted_[c]];
	else return 0;
}

const count_t Locus::getindex(count_t c) const
{
	if (c<5) return  sorted_[c];
	else return -1;
}

count_t Locus::getcount(count_t c) const
{
	count_t total=0;
	if (c<5){
		for (unsigned int s=0; s<sample.size() ;++s){
			if (sample[s].masked) continue;
			total+=sample[s].base[sorted_[c]];
		}
		return total;
	}
	else return 0;
}

count_t Locus::getcoverage(count_t s) const
{
	if (s<sample.size() ) return sample[s].base[0]+
				sample[s].base[1]+
				sample[s].base[2]+
				sample[s].base[3];
	else {
		std::cerr << "mapgd:locus.cc:137: Attempted to access a non-existent sample." << std::endl;
		exit(0);
	};
}

count_t Locus::getcoverage() const
{
	count_t total=0;
	for (unsigned int s=0; s<sample.size();++s){
		if (sample[s].masked) continue;
		total+=sample[s].base[0]+
			sample[s].base[1]+
			sample[s].base[2]+
			sample[s].base[3];
	};
	return total;
}

void Locus::set_quartet(const quartet_t &q, const count_t &c){
	sample[c]=q;
}		//sets the quartet array (unsorted)

quartet_t Locus::get_quartet(const count_t &c) const {
	return sample[c];
}
	
void Locus::set_extraid(const count_t &v, const size_t &c){
	while(extraid.size()<=c) extraid.push_back(0);
	extraid[c]=v;
};

void Locus::resize(const size_t &c){
	sample.resize(c);
};

count_t Locus::get_extraid(const size_t &c) const {
	if (extraid.size()>c) return extraid[c];
	else return 5;
}
