#ifndef GHRP_REPL_H_
#define GHRP_REPL_H_
#include "repl_policies.h"
Static RRIPclass GHRP : public ReplPolicy { 
	protected:
		/*----------------------------------------------------*/
		// prediction tables = 3 X 4096 entries X 2 bit counters = 3KB
		// prediction bits = 1 bit X 1024 blocks = 128B
		// signature = 16 bits X 1024 blocks = 2KB
		// history register = 16 bit X 1 = 2B
		/*---------------------------------------------------*/
		int m_numPredTables = 3;         // 3 prediction tables
                int m_numCounts = 4096;          // Table length
		int m_T1[m_numCounts];           // 4096 entries of 2b counters
		int m_T2[m_numCounts];           // 4096 entries of 2b counters
		int m_T3[m_numCounts];           // 4096 entries of 2b counters
		short int m_predictionBits = 64; // prediction bits (1k/16)
		short int m_signatures[1024];    // signatures (16b * 1024 blocks)
		short int m_hr;                  // history register (16b)
	public: 
		struct block { 
			bool dead; 
			int signature; 
			int tag; } blk; 
		// member memthods
		void UpdatePathHist(); 
		int Signature (); 
		void ComputeIndices(int _signature, int _indices[]); 
		int Hash(int _sign, int _Tindex); 
		int GetCounters(int _PredTables[][], int _indices, int _counters[]); 
		void UpdatePredTables(int _indices[], bool isDead); 
		void VictimBlock(struct block set[]);
        virtual void update(uint32_t id, const MemReq* req) = 0;
        virtual void replaced(uint32_t id) = 0;
};

/*------------------ Algorithm 1 -------------------------------------------*/
void
GHRP::GHRP(uint32_t _numLines, uint32_t _numCands) {
	int indices[m_numPredTables];
	int cntrsNew[m_numPredTables];
	int predTables[m_numCounts][m_numPredTables];
}

void
GHRP::update(uint32_t id, const MemReq* req) {
//this function is only called when there is a hit.
//Call updatePredTables from the hit case in Alg 1
	auto sign = Signature();
	ComputeIndices(sign, indices);
	cntrsNew = GetCounters(predTables, indices);
	bool isDead = false;
	UpdatePredTables(indices, isDead);

}

void
GHRP::Replaced(uint32_t id) {
	auto sign = Signature();
	ComputeIndices(sign, indices);
	cntrsNew = GetCounters(predTables, indices);
	bool isDead = true;
	UpdatePredTables(indices, isDead);
	auto pred = MajorityVote(cntrsNew, deadThresh); //TODO define dead threshold
	block.dead = pred;
	block.tag = tag;
}

/*-------------- Updating path history and computing signatures -------*/

void 
GHRP::UpdatePathHist() { 
	m_hr = m_hr << 4; 
	m_hr = (m_hr | m_PC) mod (pow(2,16));
}

int
GHRP::Signature() { 
	int signature = m_hr ^ m_PC; 
	return signature % pow(2,16);
}

int
GHRP::ComputeIndices(int_signature, int _indices[m_numPredTables]) { 
	for(int i = 0; i < m_numPredTables; i++) { 
		_indices[i] = Hash(_signature, i); 
	}
}

/*------------- Majority vote -----------------------------------*/

bool
GHRP::MajorityVote(int _counters[m_numPredTables], int _threshold) { 
	int vote = 0; 
	for(int i = 1; i <= m_numPredTables, i++) { 
		if(_counters[i] > _threshold) 
			vote += 1; 
	} 
	if(vote >= (m_numPredTables/2)) 
		returntrue; 
	return false;
}

/*---------------- Algorithm 4 ------------------------------*/
int 
GHRP::GetCounters(int _PredTables[][], int _ indices, int _counters[]){ 
	for(int t=1; t < m_numPredTables; t++){ 
		counters[t] = _predTables[indices[t]][t]; 
	} 
	return counters[m_numPredTables];
}

void
GHRP::UpdatePredTables(int _indices[], bool isDead){
	for(int t=1; t < m_numPredTables; t++){ 
		if (isDead){ 
			PredTables[indices[t][t]]++; 
		}
		else{ 
			PredTables[indices[t][t]]--; 
		}
	}
}

/*-------------- helper functions ----------------------------*/
intGHRP::Hash(int _signature, int _tableIndex) { 
	switch(_tableIndex) { 
case0: 
		return std::hash(_signature, m_T1); 
case1: 
		return std::hash(_signature, m_T2); 
		default: 
		return std::hash(_signature, m_T3); 
	}
}

#endif// GHRP_REPL_H_
