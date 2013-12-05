#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <algorithm>
#include <functional>

namespace task5_4
{ 
  using namespace std;

  template < typename T >
  class Deleter : public unary_function<T, void> {
  public:
    void operator()(T x) { delete x; }
  };

  template < typename T, bool Choise >
  class Deleter1 : public unary_function<T, void> {
  public:
    void operator()(T x) {}
  };

  template < typename T >
  class Deleter1 < T, false > : public unary_function<T, void> {
  public:
    void operator()(T x) { delete x.first; }
  };

  template < typename T >
  class Deleter1 < T, true > : public unary_function<T, void> {
  public:
    void operator()(T x) { delete x.second; }
  };

  template < typename T >
  class Deleter2 : public unary_function<T, void> {
  public:
    void operator()(T x) 
    {
      delete x.first;
      delete x.second;
    }
  };

  template < typename Container, bool PerformHint >
  class ContainerCleaner1
  {
  public:
    ContainerCleaner1( Container& container ) 
      : container_(container) {}
    void Perform();
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner1 < Container, false >
  {
  public:
    ContainerCleaner1( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner1 < Container, true >
  {
  public:
    ContainerCleaner1( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      for_each( container_.begin(), container_.end(), 
                Deleter<Container::value_type>() );
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

	template< bool delete_first, typename Container >
	void clear_container( Container& container )
	{
    ContainerCleaner1<Container, delete_first> cleaner(container);
    cleaner.Perform();
	}


  template < typename Container, 
    bool PerformHint1, bool PerformHint2 >
  class ContainerCleaner2
  {
  public:
    ContainerCleaner2( Container& container ) 
      : container_(container) {}
    void Perform();
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner2 < Container, false, false >
  {
  public:
    ContainerCleaner2( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner2 < Container, false, true > 
  {
  public:
    ContainerCleaner2( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      for_each( container_.begin(), container_.end(), 
                Deleter1<Container::value_type, true>() );
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner2 < Container, true, false >
  {
  public:
    ContainerCleaner2( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      for_each( container_.begin(), container_.end(), 
                Deleter1<Container::value_type, false>() );
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

  template < typename Container >
  class ContainerCleaner2 < Container, true, true > 
  {
  public:
    ContainerCleaner2( Container& container ) 
      : container_(container) {}
    void Perform()
    {
      for_each( container_.begin(), container_.end(), 
                Deleter2<Container::value_type>() );
      container_.erase( container_.begin(), container_.end() );
    }
  private:
    Container& container_;
  };

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& container )
	{
    ContainerCleaner2<Container, delete_first, delete_second> cleaner(container);
    cleaner.Perform();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

