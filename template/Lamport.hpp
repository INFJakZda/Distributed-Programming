#ifndef LAMPORT_HPP
#define LAMPORT_HPP

class Lamport {
private:
	unsigned long long time_stamp;
	
public:
	Lamport() { 
        this->time_stamp = 0;
    }
	
	unsigned long long get_time_stamp() {
		return this->time_stamp;
	}

	void set_time_stamp(unsigned long long val) {
		this->time_stamp = val;
	}

	void time_stamp_tick() { this->times_tamp++; }
};

#endif