#ifndef _LAMPORT_HPP
#define _LAMPORT_HPP

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

	void time_stamp_tick() { this->times_tamp++; }
};

#endif