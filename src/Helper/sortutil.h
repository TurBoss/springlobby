#ifndef SPRINGLOBBY_SORTUTIL_H_INCLUDED
#define SPRINGLOBBY_SORTUTIL_H_INCLUDED

template < class T>
struct CompareBase {
    typedef T CompareType;

    template < typename Type >
    inline static int compareSimple( Type o1, Type o2 ) {
        if ( o1 < o2 )
            return -1;
        else if ( o1 > o2 )
            return 1;
        return 0;
    }
};

template < class CompareImp >
struct CompareInterface {

    typedef CompareImp ImpType;

    inline ImpType& asImp(){
        return static_cast<ImpType>(*this);
    }

};

template < template <int n, bool b > class Comparator ,int C0, bool B0,int C1, bool B1,int C2, bool B2 >
struct Compare :
    public CompareInterface < Compare <Comparator ,C0, B0,C1, B1,C2, B2 > >
{
    typedef bool test;
    typedef typename Comparator<-1,false>::CompareType ObjType;
    static bool compare ( ObjType u1, ObjType u2 ) {
        assert( u1 && u2 );

        int res = Comparator<C0,B0>::compare( u1, u2 );
        if ( res != 0 )
            return res < 0;

        res = Comparator<C1,B1>::compare( u1, u2 );
        if ( res != 0 )
            return res < 0;

        res = Comparator<C2,B2>::compare( u1, u2 );
        if ( res != 0 )
            return res < 0;

        return false;

    }
};

template < template <int n, bool b > class Comparator  >
struct CompareSelector {

    typedef typename Comparator<-1,false>::CompareType ObjType;
    typedef bool  (*cmp)  ( ObjType , ObjType  )  ;

    static cmp GetFunctor( int c1, bool b1,int c2, bool b2,int c3, bool b3 )
    {

////        return  &(Compare< Comparator, 1, false, 0, true, 0, true  >::compare);
//        #define CASE_T(i) case i: return Get2<i,true>(c2,b2,c3,b3);
//        if ( b1 ) {
//            switch ( c1 ) {
////                case 1: return Get2<1,true>( c2, b2, c3, b3 );
//                CASE_T(1)
//                CASE_T(2)
//                CASE_T(3)
//                CASE_T(4)
//                CASE_T(5)
//                CASE_T(7)
//                CASE_T(8)
//                CASE_T(9)
//                CASE_T(10)
//                CASE_T(11)
//                CASE_T(12)
//                CASE_T(13)
//                CASE_T(14)
//                CASE_T(15)
//            }
//        }
//        #undef CASE_T(i)
        return  &(Compare< Comparator, 1, true, 2, true, 3, true  >::compare);

    }

    template < int C1, bool B1 >
    static cmp Get2( int c2, bool b2,int c3, bool b3 )
    {
        return  &(Compare< Comparator, C1, B1, 2, true, 3, true  >::compare);
    }

};

template< class ContainerType, class ObjType >
void SLBubbleSort( ContainerType& data, bool  (*cmp)  ( ObjType , ObjType  ) )
{
    const int n = data.size();
    int j = 0;
    bool swapped = true;
    while ( swapped )
    {
        j++;
        unsigned int swaps = 0;
        for ( int i = 0; i < n - j; ++i )
        {
            if ( cmp ( data[i], data[i+1] ) )
            {
                ObjType tmp = data[i+1];
                data[i+1] = data[i];
                data[i] = tmp;
                swapped = true;
            }
        }
    }
}

template< class ContainerType, class ObjType >
void SLInsertionSort( ContainerType& data, bool  (*cmp)  ( ObjType , ObjType  ) )
{
    const int n = data.size();
    for ( int i = 0; i < n; i++ )
    {
        ObjType v = data[i];
        int j;

        for ( j = i - 1; j >= 0; j--)
        {
            if ( cmp( data[j], v ) )
                break;
            data[j + 1] = data[j];
        }
        data[j + 1] = v;
    }

}

#endif // SPRINGLOBBY_SORTUTIL_H_INCLUDED
