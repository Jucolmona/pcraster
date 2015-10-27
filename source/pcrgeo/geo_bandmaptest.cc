#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif

#ifndef INCLUDED_GEO_BANDMAPTEST
#include "geo_bandmaptest.h"
#define INCLUDED_GEO_BANDMAPTEST
#endif

// Library headers.
#ifndef INCLUDED_BOOST_SHARED_PTR
#include <boost/shared_ptr.hpp>
#define INCLUDED_BOOST_SHARED_PTR
#endif

#ifndef INCLUDED_BOOST_TEST_TEST_TOOLS
#include <boost/test/test_tools.hpp>
#define INCLUDED_BOOST_TEST_TEST_TOOLS
#endif

#ifndef INCLUDED_BOOST_TEST_UNIT_TEST_SUITE
#include <boost/test/unit_test_suite.hpp>
#define INCLUDED_BOOST_TEST_UNIT_TEST_SUITE
#endif

// PCRaster library headers.

// Module headers.
#ifndef INCLUDED_GEO_BANDMAP
#include "geo_bandmap.h"
#define INCLUDED_GEO_BANDMAP
#endif
#ifndef INCLUDED_COM_FILE
#include "com_file.h"
#define INCLUDED_COM_FILE
#endif
#ifndef INCLUDED_GEO_CSFMAP
#include "geo_csfmap.h"
#define INCLUDED_GEO_CSFMAP
#endif
#ifndef INCLUDED_COM_CSFCELL
#include "com_csfcell.h"
#define INCLUDED_COM_CSFCELL
#endif
#ifndef INCLUDED_GEO_RASTERSPACE
#include "geo_rasterspace.h"
#define INCLUDED_GEO_RASTERSPACE
#endif
#ifndef INCLUDED_COM_EXCEPTION
#include "com_exception.h"
#define INCLUDED_COM_EXCEPTION
#endif
#ifndef INCLUDED_COM_ALGORITHM
#include "com_algorithm.h"
#define INCLUDED_COM_ALGORITHM
#endif


/*!
  \file
  This file contains the implementation of the BandMapTest class.
*/



//------------------------------------------------------------------------------
// DEFINITION OF STATIC BANDMAP MEMBERS
//------------------------------------------------------------------------------

//! suite
boost::unit_test::test_suite*geo::BandMapTest::suite()
{
  boost::unit_test::test_suite* suite = BOOST_TEST_SUITE(__FILE__);
  boost::shared_ptr<BandMapTest> instance(new BandMapTest());

  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testOpen, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testOpen2, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testCreate, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testRasterSpace, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testMultiBand, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testRead, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testPutCells, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BandMapTest::testHeader, instance));

  return suite;
}



//------------------------------------------------------------------------------
// DEFINITION OF BANDMAP MEMBERS
//------------------------------------------------------------------------------

//! ctor
geo::BandMapTest::BandMapTest()
{
}

//! setUp
void geo::BandMapTest::setUp()
{
  { // create a minimal 4*5 UINT1 map
    com::write("NROWS 4\nNCOLS 5\n",
    com::PathName("uint1minimal.hdr"));
    UINT1 buf[20];
    std::generate_n(buf,20,com::SeqInc<UINT1>());
    com::write(buf,20,com::PathName("uint1minimal.bil"));
  }
  { // create a  4*5 INT2 map with first and last cell a MV of 0
    // above 8 bits always set byteorder!
    com::write("NROWS 4\nNCOLS 5\nNBITS 16\nNODATA 0\nBYTEORDER I",
    com::PathName("int2mv0.hdr"));
    INT2 buf[20];
    std::generate_n(buf,20,com::SeqInc<INT2>());
    buf[19]=0;
    com::write(buf,20*sizeof(INT2),com::PathName("int2mv0.bil"));
  }
}

//! tearDown
void geo::BandMapTest::tearDown()
{
}

// TODO make test to 
//    - reads data from a file not big enough

void geo::BandMapTest::testOpen()
{
  setUp();

  UINT1 *buf=0;;
  try {
   BandMap bm("uint1minimal");
   BOOST_CHECK(bm.nrRows()   == 4);
   BOOST_CHECK(bm.nrCols()   == 5);
   BOOST_CHECK(bm.nrCells()  == 20);
   BOOST_CHECK(size(com::PathName("uint1minimal.bil")) ==  bm.nrCells());
   buf = new UINT1[bm.nrCells()];
   BOOST_CHECK(bm.cellSize() == 1); // default
   bm.getCellsAsUINT1(buf);
   BOOST_CHECK(!bm.mvIsSet());
   BOOST_CHECK(buf[12]==12);

   createBil("uint1minimalcpy.bil",bm.rasterSpace(), buf);
   BOOST_CHECK(com::filesExistsAndEqual("uint1minimalcpy.bil","uint1minimal.bil"));


  } catch (const com::Exception& e) {
     std::cerr << e.messages();
  }
  delete [] buf ;

  tearDown();
}

/*!
 * \todo reads and write and cmp default setting of ULYMAP and mapping that to RasterSpace
 */
void geo::BandMapTest::testRasterSpace()
{
  setUp();


  tearDown();
}

void geo::BandMapTest::testMultiBand()
{
  setUp();

  { // create a multiband 4*5 REAL4 map
    com::write("NROWS 4\nNCOLS 10\nNBANDS 3\nNBITS 32\n",
               com::PathName("mband.hdr"));
    REAL4 buf[120];
    std::generate_n(buf,120,com::SeqInc<REAL4>());
    com::write(buf,120*sizeof(REAL4),com::PathName("mband.bil"));
  }
 BandMap bm("mband");
 REAL4 *real4 = new REAL4[bm.nrCells()];
 bm.getCellsAsREAL4(real4);
 BOOST_CHECK(real4[0]==0);
 BOOST_CHECK(real4[10]==30);
 BOOST_CHECK(real4[20]==60);
 BOOST_CHECK(real4[39]==99);
 delete [] real4;

  tearDown();
}

void geo::BandMapTest::testOpen2()
{
  setUp();

 BandMap bm("int2mv0");
 BOOST_CHECK( bm.nrRows() ==4);
 BOOST_CHECK( bm.nrCols() ==5);
 BOOST_CHECK( bm.mvIsSet());
 BOOST_CHECK( bm.mvValue()==0);
 BOOST_CHECK( bm.cellRepr()==CR_INT2);

 INT2 *bufi2 = new INT2[bm.nrCells()];

 bm.getCellsRaw(bufi2);
 BOOST_CHECK(bufi2[0]==0);
 BOOST_CHECK(bufi2[11]==11);

 INT4 *bufi4 = new INT4[bm.nrCells()];
 bm.getCellsAsINT4(bufi4);
 BOOST_CHECK(bufi4[19]==MV_INT4);
 BOOST_CHECK(bufi4[0]==MV_INT4);
 BOOST_CHECK(bufi4[11]==11);

 REAL4 *real4 = new REAL4[bm.nrCells()];
 bm.getCellsAsREAL4(real4);
 BOOST_CHECK(pcr::isMV(real4[0]));
 BOOST_CHECK(real4[11]==11.0);

 delete [] bufi4;
 delete [] bufi2;
 delete [] real4;

  tearDown();
}

void geo::BandMapTest::testRead()
{
  setUp();

   // TEST SWAPPING
   // big endian, csf map with value
   // 1 everywhere
   // but first column is MV
   BandMap bm("all1_float.bil");
   BOOST_CHECK(bm.nrRows()   == 4);
   BOOST_CHECK(bm.nrCols()   == 4);
   REAL4 *buf= new REAL4[bm.nrCells()];
   BOOST_CHECK(bm.cellSize() == 1);

   bm.getCellsRaw(buf);
   BOOST_CHECK(buf[1]== 1);
   BOOST_CHECK(buf[bm.nrCells()-1]== 1);

  delete [] buf ;

  tearDown();
}

void geo::BandMapTest::testCreate()
{
  setUp();

   CSFMap  in("inp1b.map");
   RasterSpace rs(in.rasterSpace());


   UINT1 *buf= new UINT1[in.nrCells()];
   in.getCells(buf);

   createBil("inp1b",rs, buf,MV_UINT1);

   BandMap  out("inp1b");

   BOOST_CHECK(out.nrRows()==in.nrRows());
   BOOST_CHECK(out.nrCols()==in.nrCols());
   BOOST_CHECK(out.cellSize()==in.cellSize());
   BOOST_CHECK(out.cellRepr()==in.cellRepr());

   in.getCells(buf);
   BOOST_CHECK(buf[0] == MV_UINT1);
   BOOST_CHECK(buf[1] == 1);
   BOOST_CHECK(buf[24] == 1);

   BOOST_CHECK(size(com::PathName("inp1b.bil")) ==  in.nrCells());

   BandMap asInt4("inp1b");
   INT4 *bufI4 = new INT4[in.nrCells()];
   asInt4.getCellsAsINT4(bufI4);
   BOOST_CHECK(bufI4[0] == MV_INT4);
   BOOST_CHECK(bufI4[1] == 1);
   BOOST_CHECK(bufI4[24] == 1);

   delete [] bufI4;

  delete [] buf ;

  tearDown();
}

void geo::BandMapTest::testPutCells()
{
  setUp();

 {
  RasterSpace rs(4,5);
  BandMap     bm("testPutCellsputINT4",rs,CR_INT4,false,0);
  INT4 createBuf[20];
  std::generate_n(createBuf,20,com::SeqInc<INT4>());
  createBuf[0] =MV_INT4;
  // test truncation/sign wrap due to INT2 storage
  createBuf[10]=INT2_MAX+1+99;

  bm.putCellsAsINT4(createBuf);

  BandMap     readBm("testPutCellsputINT4");
  BOOST_CHECK(readBm.cellRepr() == CR_INT2);
  BOOST_CHECK(readBm.nrRows() == 4);
  BOOST_CHECK(readBm.nrCols() == 5);
  INT4 readBuf[20];
  readBm.getCellsAsINT4(readBuf);
  BOOST_CHECK(readBuf[0]==MV_INT4);
  BOOST_CHECK(readBuf[2]==2);
  // test truncation/sign wrap due to INT2 storage
  BOOST_CHECK(readBuf[10]==INT2_MIN-1+99);
 }
 {
  RasterSpace rs(4,5);
  BandMap     bm("testPutCellsputREAL4",rs,CR_REAL4,false,0);
  REAL4 createBuf[20];
  std::generate_n(createBuf,20,com::SeqInc<REAL4>());
  pcr::setMV(createBuf[0]);

  bm.putCellsAsREAL4(createBuf);

  BandMap     readBm("testPutCellsputREAL4");
  BOOST_CHECK(readBm.cellRepr() == CR_REAL4);
  BOOST_CHECK(readBm.nrRows() == 4);
  BOOST_CHECK(readBm.nrCols() == 5);
  REAL4 readBuf[20];

  readBm.getCellsRaw(readBuf);
  BOOST_CHECK(readBuf[0]==-999);
  BOOST_CHECK(readBuf[2]==2);

  REAL4 readBuf2[20];
  readBm.getCellsAsREAL4(readBuf2);
  BOOST_CHECK(pcr::isMV(readBuf2[0]));
  BOOST_CHECK(readBuf2[2]==2);

  // test stx
  com::PathName stx("testPutCellsputREAL4.stx");
  std::string stxContents;
  com::read(stxContents,stx);
  BOOST_CHECK(stxContents == "1 1 19\n");
 }

  tearDown();
}

void geo::BandMapTest::testHeader()
{
  setUp();

  UINT1 buf[20];
  std::generate_n(buf,20,com::SeqInc<UINT1>());
  com::write(buf,20,com::PathName("headertest.bil"));
  com::PathName pn("headertest.hdr");

  // comments are allowed
  // unknown keys are discarded
  bool succes=true;
  try {
    com::write("NROWS 4 A COMMENT\nNCOLS 5\nUNKNOWNKEY 5\n",pn);
    BandMap bm("headertest");
  } catch(...) {
    succes=false;
  }
  BOOST_CHECK(succes);

  // NROWS and NCOLS are required
  bool failure=false;
  try {
    com::write("XDIM 1\nUNKNOWNKEY 5\n",pn);
    BandMap bm("headertest");
  } catch(const com::Exception& e) {
    BOOST_CHECK(e.messages().find("NROWS") != std::string::npos
           || e.messages().find("NCOLS") != std::string::npos);
    failure=true;
  }
  BOOST_CHECK(failure);

  // allow ommision of last new line
  succes=true;
  try {
    com::write("NROWS 4\nNCOLS 5",pn);
    BandMap bm("headertest");
  } catch(...) {
    succes=false;
  }
  BOOST_CHECK(succes);

  {
    // if both dim are set, then ok
    com::write("NROWS 4\nNCOLS 5\nXDIM 0.5\nYDIM 0.5", pn);
    BandMap bm("headertest");
    BOOST_CHECK(bm.cellSize()==0.5); // as set
  }

  {
    // if only one dim then the default is set
    com::write("NROWS 4\nNCOLS 5\nXDIM 0.5\n", pn);
    BandMap bm("headertest");
    BOOST_CHECK(bm.cellSize()==1); // the default
  }

  // if both DIM set they must be equal
  failure=false;
  try {
    com::write("NROWS 3\nNCOLS 4\nXDIM 1\nYDIM 0.5\n",pn);
    BandMap bm("headertest");
  } catch(const com::Exception& e) {
    BOOST_CHECK(e.messages().find("XDIM") != std::string::npos
           && e.messages().find("YDIM") != std::string::npos);
    failure=true;
  }
  BOOST_CHECK(failure);

  tearDown();
}