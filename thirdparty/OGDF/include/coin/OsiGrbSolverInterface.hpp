//-----------------------------------------------------------------------------
// name:     OSI Interface for Gurobi
// template: OSI Cplex Interface written by T. Achterberg
// author:   Stefan Vigerske
//           Humboldt University Berlin
// date:     09/02/2009
// license:  this file may be freely distributed under the terms of EPL
// comments: please scan this file for '???' and read the comments
//-----------------------------------------------------------------------------
// Copyright (C) 2009 Humboldt University Berlin and others.
// All Rights Reserved.

// $Id: OsiGrbSolverInterface.hpp 1754 2011-06-19 16:07:57Z stefan $

#ifndef OsiGrbSolverInterface_H
#define OsiGrbSolverInterface_H

#include <string>
#include "OsiSolverInterface.hpp"

typedef struct _GRBmodel GRBmodel;
typedef struct _GRBenv GRBenv;

/** Gurobi Solver Interface

 Instantiation of OsiGrbSolverInterface for Gurobi
 */

class OsiGrbSolverInterface : virtual public OsiSolverInterface {
  friend void OsiGrbSolverInterfaceUnitTest(const std::string & mpsDir, const std::string & netlibDir);

public:

	//---------------------------------------------------------------------------
	/**@name Solve methods */
	//@{
	/// Solve initial LP relaxation
	virtual void initialSolve() override;

	/// Resolve an LP relaxation after problem modification
	virtual void resolve() override;

	/// Invoke solver's built-in enumeration algorithm
	virtual void branchAndBound() override;
	//@}

	//---------------------------------------------------------------------------
	/**@name Parameter set/get methods

	 The set methods return true if the parameter was set to the given value,
	 false otherwise. There can be various reasons for failure: the given
	 parameter is not applicable for the solver (e.g., refactorization
	 frequency for the volume algorithm), the parameter is not yet implemented
	 for the solver or simply the value of the parameter is out of the range
	 the solver accepts. If a parameter setting call returns false check the
	 details of your solver.

	 The get methods return true if the given parameter is applicable for the
	 solver and is implemented. In this case the value of the parameter is
	 returned in the second argument. Otherwise they return false.
	 */
	//@{
	// Set an integer parameter
	bool setIntParam(OsiIntParam key, int value) override;
	// Set an double parameter
	bool setDblParam(OsiDblParam key, double value) override;
	// Set a string parameter
	bool setStrParam(OsiStrParam key, const std::string & value) override;
	// Set a hint parameter
	bool setHintParam(OsiHintParam key, bool yesNo = true, OsiHintStrength strength = OsiHintTry, void* = nullptr) override;
	// Get an integer parameter
	bool getIntParam(OsiIntParam key, int& value) const override;
	// Get an double parameter
	bool getDblParam(OsiDblParam key, double& value) const override;
	// Get a string parameter
	bool getStrParam(OsiStrParam key, std::string& value) const override;
	// Get a hint parameter
	bool getHintParam(OsiHintParam key, bool& yesNo, OsiHintStrength& strength, void*& otherInformation) const override;
	// Get a hint parameter
	bool getHintParam(OsiHintParam key, bool& yesNo, OsiHintStrength& strength) const override;
	// Get a hint parameter
	bool getHintParam(OsiHintParam key, bool& yesNo) const override;
  // Set mipstart option (pass column solution to CPLEX before MIP start)
  void setMipStart(bool value) { domipstart = value; }
  // Get mipstart option value
  bool getMipStart() const { return domipstart; }
	//@}

	//---------------------------------------------------------------------------
	///@name Methods returning info on how the solution process terminated
	//@{
	/// Are there a numerical difficulties?
	virtual bool isAbandoned() const override;
	/// Is optimality proven?
	virtual bool isProvenOptimal() const override;
	/// Is primal infeasiblity proven?
	virtual bool isProvenPrimalInfeasible() const override;
	/// Is dual infeasiblity proven?
	virtual bool isProvenDualInfeasible() const override;
	/// Is the given primal objective limit reached?
	virtual bool isPrimalObjectiveLimitReached() const override;
	/// Is the given dual objective limit reached?
	virtual bool isDualObjectiveLimitReached() const override;
	/// Iteration limit reached?
	virtual bool isIterationLimitReached() const override;
	//@}

	//---------------------------------------------------------------------------
	/**@name WarmStart related methods */
	//@{

	/*! \brief Get an empty warm start object

	 This routine returns an empty CoinWarmStartBasis object. Its purpose is
	 to provide a way to give a client a warm start basis object of the
	 appropriate type, which can resized and modified as desired.
	 */
	CoinWarmStart* getEmptyWarmStart() const override;

	/// Get warmstarting information
	virtual CoinWarmStart* getWarmStart() const override;
	/** Set warmstarting information. Return true/false depending on whether
	 the warmstart information was accepted or not. */
	virtual bool setWarmStart(const CoinWarmStart* warmstart) override;
	//@}

	//---------------------------------------------------------------------------
	/**@name Hotstart related methods (primarily used in strong branching). <br>
	 The user can create a hotstart (a snapshot) of the optimization process
	 then reoptimize over and over again always starting from there.<br>
	 <strong>NOTE</strong>: between hotstarted optimizations only
	 bound changes are allowed. */
	//@{
	/// Create a hotstart point of the optimization process
	virtual void markHotStart() override;
	/// Optimize starting from the hotstart
	virtual void solveFromHotStart() override;
	/// Delete the snapshot
	virtual void unmarkHotStart() override;
	//@}

	//---------------------------------------------------------------------------
	/**@name Problem information methods

	 These methods call the solver's query routines to return
	 information about the problem referred to by the current object.
	 Querying a problem that has no data associated with it result in
	 zeros for the number of rows and columns, and NULL pointers from
	 the methods that return vectors.

	 Const pointers returned from any data-query method are valid as
	 long as the data is unchanged and the solver is not called.
	 */
	//@{
	/**@name Methods related to querying the input data */
	//@{
	/// Get number of columns
	virtual int getNumCols() const override;

	/// Get number of rows
	virtual int getNumRows() const override;

	/// Get number of nonzero elements
	virtual int getNumElements() const override;

	/// Get pointer to array[getNumCols()] of column lower bounds
	virtual const double * getColLower() const override;

	/// Get pointer to array[getNumCols()] of column upper bounds
	virtual const double * getColUpper() const override;

	/** Get pointer to array[getNumRows()] of row constraint senses.
	 <ul>
	 <li>'L': <= constraint
	 <li>'E': =  constraint
	 <li>'G': >= constraint
	 <li>'R': ranged constraint
	 <li>'N': free constraint
	 </ul>
	 */
	virtual const char * getRowSense() const override;

	/** Get pointer to array[getNumRows()] of rows right-hand sides
	 <ul>
	 <li> if rowsense()[i] == 'L' then rhs()[i] == rowupper()[i]
	 <li> if rowsense()[i] == 'G' then rhs()[i] == rowlower()[i]
	 <li> if rowsense()[i] == 'R' then rhs()[i] == rowupper()[i]
	 <li> if rowsense()[i] == 'N' then rhs()[i] == 0.0
	 </ul>
	 */
	virtual const double * getRightHandSide() const override;

	/** Get pointer to array[getNumRows()] of row ranges.
	 <ul>
	 <li> if rowsense()[i] == 'R' then
	 rowrange()[i] == rowupper()[i] - rowlower()[i]
	 <li> if rowsense()[i] != 'R' then
	 rowrange()[i] is 0.0
	 </ul>
	 */
	virtual const double * getRowRange() const override;

	/// Get pointer to array[getNumRows()] of row lower bounds
	virtual const double * getRowLower() const override;

	/// Get pointer to array[getNumRows()] of row upper bounds
	virtual const double * getRowUpper() const override;

	/// Get pointer to array[getNumCols()] of objective function coefficients
	virtual const double * getObjCoefficients() const override;

	/// Get objective function sense (1 for min (default), -1 for max)
	virtual double getObjSense() const override;

	/// Return true if column is continuous
	virtual bool isContinuous(int colNumber) const override;

	/// Get pointer to row-wise copy of matrix
	virtual const CoinPackedMatrix * getMatrixByRow() const override;

	/// Get pointer to column-wise copy of matrix
	virtual const CoinPackedMatrix * getMatrixByCol() const override;

	/// Get solver's value for infinity
	virtual double getInfinity() const override;
	//@}

	/**@name Methods related to querying the solution */
	//@{
	/// Get pointer to array[getNumCols()] of primal solution vector
	virtual const double * getColSolution() const override;

	/// Get pointer to array[getNumRows()] of dual prices
	virtual const double * getRowPrice() const override;

	/// Get a pointer to array[getNumCols()] of reduced costs
	virtual const double * getReducedCost() const override;

	/** Get pointer to array[getNumRows()] of row activity levels (constraint
	 matrix times the solution vector */
	virtual const double * getRowActivity() const override;

	/// Get objective function value
	virtual double getObjValue() const override;

	/** Get how many iterations it took to solve the problem (whatever
	 "iteration" mean to the solver. */
	virtual int getIterationCount() const override;

	/** Get as many dual rays as the solver can provide. (In case of proven
	 primal infeasibility there should be at least one.)

	 The first getNumRows() ray components will always be associated with
	 the row duals (as returned by getRowPrice()). If \c fullRay is true,
	 the final getNumCols() entries will correspond to the ray components
	 associated with the nonbasic variables. If the full ray is requested
	 and the method cannot provide it, it will throw an exception.

	 <strong>NOTE for implementers of solver interfaces:</strong> <br>
	 The double pointers in the vector should point to arrays of length
	 getNumRows() and they should be allocated via new[]. <br>

	 <strong>NOTE for users of solver interfaces:</strong> <br>
	 It is the user's responsibility to free the double pointers in the
	 vector using delete[].
	 */
	virtual std::vector<double*> getDualRays(int maxNumRays,
						 bool fullRay=false) const override;
	/** Get as many primal rays as the solver can provide. (In case of proven
	 dual infeasibility there should be at least one.)

	 <strong>NOTE for implementers of solver interfaces:</strong> <br>
	 The double pointers in the vector should point to arrays of length
	 getNumCols() and they should be allocated via new[]. <br>

	 <strong>NOTE for users of solver interfaces:</strong> <br>
	 It is the user's responsibility to free the double pointers in the
	 vector using delete[].
	 */
	virtual std::vector<double*> getPrimalRays(int maxNumRays) const override;
	//@}
	//@}

	//---------------------------------------------------------------------------

	/**@name Problem modifying methods */
	//@{
	//-------------------------------------------------------------------------
	/**@name Changing bounds on variables and constraints */
	//@{
	/** Set an objective function coefficient */
	virtual void setObjCoeff(int elementIndex, double elementValue) override;

	/** Set a a set of objective function coefficients */
	virtual void setObjCoeffSet(const int* indexFirst, const int* indexLast,
			const double* coeffList) override;

	using OsiSolverInterface::setColLower;
	/** Set a single column lower bound<br>
	 Use -COIN_DBL_MAX for -infinity. */
	virtual void setColLower(int elementIndex, double elementValue) override;

	using OsiSolverInterface::setColUpper;
	/** Set a single column upper bound<br>
	 Use COIN_DBL_MAX for infinity. */
	virtual void setColUpper(int elementIndex, double elementValue) override;

	/** Set a single column lower and upper bound<br>
	 The default implementation just invokes <code>setColLower()</code> and
	 <code>setColUpper()</code> */
	virtual void setColBounds(int elementIndex, double lower, double upper) override;

	/** Set the bounds on a number of columns simultaneously<br>
	 The default implementation just invokes <code>setCollower()</code> and
	 <code>setColupper()</code> over and over again.
	 @param <code>[indexfirst,indexLast]</code> contains the indices of
	 the constraints whose </em>either</em> bound changes
	 @param boundList the new lower/upper bound pairs for the variables
	 */
	virtual void setColSetBounds(const int* indexFirst, const int* indexLast,
			const double* boundList) override;

	/** Set a single row lower bound<br>
	 Use -COIN_DBL_MAX for -infinity. */
	virtual void setRowLower(int elementIndex, double elementValue) override;

	/** Set a single row upper bound<br>
	 Use COIN_DBL_MAX for infinity. */
	virtual void setRowUpper(int elementIndex, double elementValue) override;

	/** Set a single row lower and upper bound<br>
	 The default implementation just invokes <code>setRowLower()</code> and
	 <code>setRowUpper()</code> */
	virtual void setRowBounds(int elementIndex, double lower, double upper) override;

	/** Set the type of a single row<br> */
	virtual void setRowType(int index, char sense, double rightHandSide,
			double range) override;

	/** Set the bounds on a number of rows simultaneously<br>
	 The default implementation just invokes <code>setRowLower()</code> and
	 <code>setRowUpper()</code> over and over again.
	 @param <code>[indexfirst,indexLast]</code> contains the indices of
	 the constraints whose </em>either</em> bound changes
	 @param boundList the new lower/upper bound pairs for the constraints
	 */
	virtual void setRowSetBounds(const int* indexFirst, const int* indexLast,
			const double* boundList) override;

	/** Set the type of a number of rows simultaneously<br>
	 The default implementation just invokes <code>setRowType()</code> and
	 over and over again.
	 @param <code>[indexfirst,indexLast]</code> contains the indices of
	 the constraints whose type changes
	 @param senseList the new senses
	 @param rhsList   the new right hand sides
	 @param rangeList the new ranges
	 */
	virtual void setRowSetTypes(const int* indexFirst, const int* indexLast,
			const char* senseList, const double* rhsList, const double* rangeList) override;
	//@}

	//-------------------------------------------------------------------------
	/**@name Integrality related changing methods */
	//@{
	/** Set the index-th variable to be a continuous variable */
	virtual void setContinuous(int index) override;
	/** Set the index-th variable to be an integer variable */
	virtual void setInteger(int index) override;
	/** Set the variables listed in indices (which is of length len) to be
	 continuous variables */
	virtual void setContinuous(const int* indices, int len) override;
	/** Set the variables listed in indices (which is of length len) to be
	 integer variables */
	virtual void setInteger(const int* indices, int len) override;
	//@}

  //-------------------------------------------------------------------------
  /**@name Naming methods */
  //@{
  /*! \brief Set a row name */
  virtual void setRowName(int ndx, std::string name) override ;

  /*! \brief Set a column name */
  virtual void setColName(int ndx, std::string name) override ;
  //@}

	//-------------------------------------------------------------------------
	/// Set objective function sense (1 for min (default), -1 for max,)
	virtual void setObjSense(double s) override;

	/** Set the primal solution column values

	 colsol[numcols()] is an array of values of the problem column
	 variables. These values are copied to memory owned by the
	 solver object or the solver.  They will be returned as the
	 result of colsol() until changed by another call to
	 setColsol() or by a call to any solver routine.  Whether the
	 solver makes use of the solution in any way is
	 solver-dependent.
	 */
	virtual void setColSolution(const double * colsol) override;

	/** Set dual solution vector

	 rowprice[numrows()] is an array of values of the problem row
	 dual variables. These values are copied to memory owned by the
	 solver object or the solver.  They will be returned as the
	 result of rowprice() until changed by another call to
	 setRowprice() or by a call to any solver routine.  Whether the
	 solver makes use of the solution in any way is
	 solver-dependent.
	 */
	virtual void setRowPrice(const double * rowprice) override;

	//-------------------------------------------------------------------------
	/**@name Methods to expand a problem.<br>
	 Note that if a column is added then by default it will correspond to a
	 continuous variable. */
	//@{
	using OsiSolverInterface::addCol;
	/** */
	virtual void addCol(const CoinPackedVectorBase& vec, const double collb,
			const double colub, const double obj) override;

	using OsiSolverInterface::addCols;
	/** */
	virtual void addCols(const int numcols,
			const CoinPackedVectorBase * const * cols, const double* collb,
			const double* colub, const double* obj) override;
	/** */
	virtual void deleteCols(const int num, const int * colIndices) override;

	using OsiSolverInterface::addRow;
	/** */
	virtual void addRow(const CoinPackedVectorBase& vec, const double rowlb,
			const double rowub) override;
	/** */
	virtual void addRow(const CoinPackedVectorBase& vec, const char rowsen,
			const double rowrhs, const double rowrng) override;

	using OsiSolverInterface::addRows;
	/** */
	virtual void addRows(const int numrows,
			const CoinPackedVectorBase * const * rows, const double* rowlb,
			const double* rowub) override;
	/** */
	virtual void addRows(const int numrows,
			const CoinPackedVectorBase * const * rows, const char* rowsen,
			const double* rowrhs, const double* rowrng) override;
	/** */
	virtual void deleteRows(const int num, const int * rowIndices) override;
	//@}
	//@}

	//---------------------------------------------------------------------------

	/**@name Methods to input a problem */
	//@{
	/** Load in an problem by copying the arguments (the constraints on the
	 rows are given by lower and upper bounds). If a pointer is 0 then the
	 following values are the default:
	 <ul>
	 <li> <code>colub</code>: all columns have upper bound infinity
	 <li> <code>collb</code>: all columns have lower bound 0
	 <li> <code>rowub</code>: all rows have upper bound infinity
	 <li> <code>rowlb</code>: all rows have lower bound -infinity
	 <li> <code>obj</code>: all variables have 0 objective coefficient
	 </ul>
	 */
	virtual void loadProblem(const CoinPackedMatrix& matrix, const double* collb,
			const double* colub, const double* obj, const double* rowlb,
			const double* rowub) override;

	/** Load in an problem by assuming ownership of the arguments (the
	 constraints on the rows are given by lower and upper bounds). For
	 default values see the previous method. <br>
	 <strong>WARNING</strong>: The arguments passed to this method will be
	 freed using the C++ <code>delete</code> and <code>delete[]</code>
	 functions.
	 */
	virtual void assignProblem(CoinPackedMatrix*& matrix, double*& collb,
			double*& colub, double*& obj, double*& rowlb, double*& rowub) override;

	/** Load in an problem by copying the arguments (the constraints on the
	 rows are given by sense/rhs/range triplets). If a pointer is 0 then the
	 following values are the default:
	 <ul>
	 <li> <code>colub</code>: all columns have upper bound infinity
	 <li> <code>collb</code>: all columns have lower bound 0
	 <li> <code>obj</code>: all variables have 0 objective coefficient
	 <li> <code>rowsen</code>: all rows are >=
	 <li> <code>rowrhs</code>: all right hand sides are 0
	 <li> <code>rowrng</code>: 0 for the ranged rows
	 </ul>
	 */
	virtual void loadProblem(const CoinPackedMatrix& matrix, const double* collb,
			const double* colub, const double* obj, const char* rowsen,
			const double* rowrhs, const double* rowrng) override;

	/** Load in an problem by assuming ownership of the arguments (the
	 constraints on the rows are given by sense/rhs/range triplets). For
	 default values see the previous method. <br>
	 <strong>WARNING</strong>: The arguments passed to this method will be
	 freed using the C++ <code>delete</code> and <code>delete[]</code>
	 functions.
	 */
	virtual void assignProblem(CoinPackedMatrix*& matrix, double*& collb,
			double*& colub, double*& obj, char*& rowsen, double*& rowrhs,
			double*& rowrng) override;

	/** Just like the other loadProblem() methods except that the matrix is
	 given in a standard column major ordered format (without gaps). */
	virtual void loadProblem(const int numcols, const int numrows,
			const int* start, const int* index, const double* value,
			const double* collb, const double* colub, const double* obj,
			const double* rowlb, const double* rowub) override;

	/** Just like the other loadProblem() methods except that the matrix is
	 given in a standard column major ordered format (without gaps). */
	virtual void loadProblem(const int numcols, const int numrows,
			const int* start, const int* index, const double* value,
			const double* collb, const double* colub, const double* obj,
			const char* rowsen, const double* rowrhs, const double* rowrng) override;

	using OsiSolverInterface::readMps;
	/** Read an mps file from the given filename */
	virtual int readMps(const char *filename, const char *extension = "mps") override;

	/** Write the problem into an mps file of the given filename.
	 If objSense is non zero then -1.0 forces the code to write a
	 maximization objective and +1.0 to write a minimization one.
	 If 0.0 then solver can do what it wants */
	virtual void writeMps(const char *filename, const char *extension = "mps",
			double objSense=0.0) const override;

	//@}

	//---------------------------------------------------------------------------

	/**@name Gurobi specific public interfaces */
	//@{
	/** Get pointer to Gurobi model and free all specified cached data entries
	 (combined with logical or-operator '|' ):
	 */
	enum keepCachedFlag
	{
		/// discard all cached data (default)
		KEEPCACHED_NONE = 0,
		/// column information: objective values, lower and upper bounds, variable types
		KEEPCACHED_COLUMN = 1,
		/// row information: right hand sides, ranges and senses, lower and upper bounds for row
		KEEPCACHED_ROW = 2,
		/// problem matrix: matrix ordered by column and by row
		KEEPCACHED_MATRIX = 4,
		/// LP solution: primal and dual solution, reduced costs, row activities
		KEEPCACHED_RESULTS = 8,
		/// only discard cached LP solution
		KEEPCACHED_PROBLEM = KEEPCACHED_COLUMN | KEEPCACHED_ROW | KEEPCACHED_MATRIX,
		/// keep all cached data (similar to getMutableLpPtr())
		KEEPCACHED_ALL = KEEPCACHED_PROBLEM | KEEPCACHED_RESULTS,
		/// free only cached column and LP solution information
		FREECACHED_COLUMN = KEEPCACHED_PROBLEM & ~KEEPCACHED_COLUMN,
		/// free only cached row and LP solution information
		FREECACHED_ROW = KEEPCACHED_PROBLEM & ~KEEPCACHED_ROW,
		/// free only cached matrix and LP solution information
		FREECACHED_MATRIX = KEEPCACHED_PROBLEM & ~KEEPCACHED_MATRIX,
		/// free only cached LP solution information
		FREECACHED_RESULTS = KEEPCACHED_ALL & ~KEEPCACHED_RESULTS
	};

	GRBmodel* getLpPtr(int keepCached = KEEPCACHED_NONE);

	//@{
	/// Method to access Gurobi environment pointer
	GRBenv* getEnvironmentPtr() const;

  /// Return whether the current Gurobi environment runs in demo mode.
  bool isDemoLicense() const;
	//@}

	/// return a vector of variable types (continous, binary, integer)
	const char* getCtype() const;

	/**@name Static instance counter methods */
	/** Gurobi has a context which must be created prior to all other Gurobi calls.
	 This method:
	 <ul>
	 <li>Increments by 1 the number of uses of the Gurobi environment.
	 <li>Creates the Gurobi context when the number of uses is change to
	 1 from 0.
	 </ul>
	 */
	static void incrementInstanceCounter();

	/** Gurobi has a context which should be deleted after Gurobi calls.
	 This method:
	 <ul>
	 <li>Decrements by 1 the number of uses of the Gurobi environment.
	 <li>Deletes the Gurobi context when the number of uses is change to
	 0 from 1.
	 </ul>
	 */
	static void decrementInstanceCounter();

	/// sets the global gurobi environment to a user given one
	static void setEnvironment(GRBenv* globalenv);

	/// Return the number of instances of instantiated objects using Gurobi services.
	static unsigned int getNumInstances();
	//@}
	//@}

	/**@name Constructors and destructor */
	//@{
	/// Default Constructor
	OsiGrbSolverInterface(bool use_local_env = false);

	/// Constructor that takes a gurobi environment and assumes membership
	OsiGrbSolverInterface(GRBenv* localgrbenv);

	/// Clone
	virtual OsiSolverInterface * clone(bool copyData = true) const override;

	/// Copy constructor
	OsiGrbSolverInterface(const OsiGrbSolverInterface&);

	/// Assignment operator
	OsiGrbSolverInterface& operator=(const OsiGrbSolverInterface& rhs);

	/// Destructor
	virtual ~OsiGrbSolverInterface();

	/// Resets as if default constructor
	virtual void reset() override;
	//@}

	/***************************************************************************/
	/**@name OsiSimplexInterface methods
	 Gurobi adds a slack with coeff +1 in "<=" and "=" constraints,
	 with coeff -1 in ">=",
	 slack being non negative. We switch in order to get a "Clp tableau"
	 where all the slacks have coefficient +1 in the original tableau.

	 If a slack for ">=" is non basic, invB is not changed;
	 column of the slack in the optimal tableau is flipped.

	 If a slack for ">=" is basic, corresp. row of invB is flipped;
	 whole row of the optimal tableau is flipped;
	 then whole column for the slack in opt tableau is flipped.

	 Ranged rows are not supported. It might work, but no garantee is given.
	 */
	//@{

	/** Returns 1 if can just do getBInv etc
	 2 if has all OsiSimplex methods
	 and 0 if it has none */
	virtual int canDoSimplexInterface() const override;

	using OsiSolverInterface::enableSimplexInterface;
	/** Useless function, defined only for compatibility with
	 OsiSimplexInterface
	 */
	virtual void enableSimplexInterface(int doingPrimal) {
	}
	;

	/** Useless function, defined only for compatibility with
	 OsiSimplexInterface
	 */
	virtual void disableSimplexInterface() override {
	}
	;

	/** Useless function, defined only for compatibility with
	 OsiSimplexInterface
	 */
	virtual void enableFactorization() const override {
	}
	;

	/** Useless function, defined only for compatibility with
	 OsiSimplexInterface
	 */
	virtual void disableFactorization() const override {
	}
	;

	///Returns true if a basis is available
	virtual bool basisIsAvailable() const override;

	/** Returns a basis status of the structural/artificial variables
	 At present as warm start i.e 0: free, 1: basic, 2: upper, 3: lower
	 */
	virtual void getBasisStatus(int* cstat, int* rstat) const override;

	//  ///Get a row of the tableau (slack part in slack if not NULL)
	//  virtual void getBInvARow(int row, double* z, double * slack=NULL) const;
	//
	//  ///Get a row of the basis inverse
	//  virtual void getBInvRow(int row, double* z) const;
	//
	//  ///Get a column of the tableau
	//  virtual void getBInvACol(int col, double* vec) const;
	//
	//  ///Get a column of the basis inverse
	//  virtual void getBInvCol(int col, double* vec) const;
	//
	//  /**  Get indices of the pivot variable in each row
	//      (order of indices corresponds to the
	//      order of elements in a vector retured by getBInvACol() and
	//      getBInvCol()).
	//  */
	//  virtual void getBasics(int* index) const;
	/// switches Gurobi to prob type LP
	void switchToLP();

	/// switches Gurobi to prob type MIP
	void switchToMIP();

	//@}
	/***************************************************************************/

protected:

	/**@name Protected methods */
	//@{
	/// Apply a row cut. Return true if cut was applied.
	virtual void applyRowCut(const OsiRowCut & rc) override;

	/** Apply a column cut (bound adjustment).
	 Return true if cut was applied.
	 */
	virtual void applyColCut(const OsiColCut & cc) override;
	//@}

private:
	/**@name Private static class functions  */
	//@{
	/// resizes coltype_, colmap_O2G, colmap_G2O vectors to be able to store at least minsize elements
	void resizeColSpace(int minsize);

	/// frees colsize_ vector
	void freeColSpace();

	/// resizes colmap_G2O vector to be able to store at least minsize (auxiliary) elements
	void resizeAuxColSpace(int minsize);

	/// resizes auxcolind vector to current number of rows and inits values to -1
	void resizeAuxColIndSpace();
	//@}

	/**@name Private static class data */
	//@{
	/// Gurobi environment pointer
	static GRBenv* globalenv_;
	/// whether OsiGrb has created the global environment (and thus should free it)
	static bool globalenv_is_ours;
	/// Number of instances using the global Gurobi environment
	static unsigned int numInstances_;
	//@}

	/**@name Private methods */
	//@{

	/// Get LP Pointer for const methods
	GRBmodel* getMutableLpPtr() const;

	/// The real work of a copy constructor (used by copy and assignment)
	void gutsOfCopy(const OsiGrbSolverInterface & source);

	/// The real work of the constructor
	void gutsOfConstructor();

	/// The real work of the destructor
	void gutsOfDestructor();

	/// free cached column rim vectors
	void freeCachedColRim();

	/// free cached row rim vectors
	void freeCachedRowRim();

	/// free cached result vectors
	void freeCachedResults();

	/// free cached matrices
	void freeCachedMatrix();

	/// free all cached data (except specified entries, see getLpPtr())
	void freeCachedData(int keepCached = KEEPCACHED_NONE);

	/// free all allocated memory
	void freeAllMemory();

	/// converts a normal row into a ranged row by adding an auxiliary variable
	void convertToRangedRow(int rowidx, double rhs, double range);

	/// converts a ranged row into a normal row by removing its auxiliary variable
	void convertToNormalRow(int rowidx, char sense, double rhs);
	//@}


	/**@name Private member data */
	//@{
	/// Gurobi environment used only by this class instance
	mutable GRBenv* localenv_;

	/// Gurobi model represented by this class instance
	mutable GRBmodel* lp_;

	/// Hotstart information
	int *hotStartCStat_;
	int hotStartCStatSize_;
	int *hotStartRStat_;
	int hotStartRStatSize_;
	int hotStartMaxIteration_;

  /// OSI name discipline
  int nameDisc_;

	/**@name Cached information derived from the Gurobi model */
	//@{
	/// Pointer to objective vector
	mutable double *obj_;

	/// Pointer to dense vector of variable lower bounds
	mutable double *collower_;

	/// Pointer to dense vector of variable lower bounds
	mutable double *colupper_;

	/// Pointer to dense vector of row sense indicators
	mutable char *rowsense_;

	/// Pointer to dense vector of row right-hand side values
	mutable double *rhs_;

	/// Pointer to dense vector of slack upper bounds for range constraints (undefined for non-range rows)
	mutable double *rowrange_;

	/// Pointer to dense vector of row lower bounds
	mutable double *rowlower_;

	/// Pointer to dense vector of row upper bounds
	mutable double *rowupper_;

	/// Pointer to primal solution vector
	mutable double *colsol_;

	/// Pointer to dual solution vector
	mutable double *rowsol_;

	/// Pointer to reduced cost vector
	mutable double *redcost_;

	/// Pointer to row activity (slack) vector
	mutable double *rowact_;

	/// Pointer to row-wise copy of problem matrix coefficients.
	mutable CoinPackedMatrix *matrixByRow_;

	/// Pointer to row-wise copy of problem matrix coefficients.
	mutable CoinPackedMatrix *matrixByCol_;
	//@}

	/**@name Additional information needed for storing MIP problems and handling ranged rows */
	//@{
  /// Stores whether we currently see the problem as a MIP
  mutable bool probtypemip_;

  /// Whether to pass a column solution to CPLEX before starting MIP solve (copymipstart)
  bool domipstart;

	/// Size of allocated memory for coltype_, colmap_O2G, and (with offset auxcolspace) colmap_G2O.
  int colspace_;

	/// Pointer to dense vector of variable types (continous, binary, integer)
	char *coltype_;

	/// Number of auxiliary columns in Gurobi model for handling of ranged rows
	int nauxcols;

	/// Size of allocated memory for colmap_G2O that exceeds colspace_
	int auxcolspace;

	/// Maps variable indices from Osi to Gurobi
	/// Is NULL if there are no ranged rows! (assume identity mapping then)
	int* colmap_O2G;

	/// Maps variable indices from Gurobi to Osi
	/// A negative value indicates that a variable is an auxiliary variable that was added to handle a ranged row
	/// -colmap_G2O[i]-1 gives the index of the ranged row in this case
  /// Is NULL if there are no ranged rows! (assume identity mapping then)
	int* colmap_G2O;

	/// Current length of auxcolind array.
	int auxcolindspace;

	/// Gives for each row the index of the corresponding auxiliary variable, if it is a ranged row.
	/// Otherwise, gives -1.
  /// Is NULL if there are no ranged rows! (assume -1 for each row then)
	int* auxcolind;
  //@}
};

//#############################################################################
/** A function that tests the methods in the OsiGrbSolverInterface class. */
void OsiGrbSolverInterfaceUnitTest(const std::string & mpsDir, const std::string & netlibDir);

#endif
