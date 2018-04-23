#ifndef LAMPORT_HPP
#define LAMPORT_HPP

class Lamport {
private:
	long long int timestamp;
	int rank;
	void timestamp_tick() {timestamp++;}

public:
	Lamport(int rank, long long initial = 0){ 
        this->timestamp = _initial;
        this->rank = _rank;
    }
	
	long long int get_timestamp() {
		return this->timestamp;
	}

	int get_rank() {
		return this->rank;
	}

	long long receive_timestamp(long long foreign_timestamp) {
		this->timestamp = this->timestamp > foreign_timestamp ? this->timestamp : foreign_timestamp;
		timestamp_tick();
		return this->timestamp;
	}
};

#endif