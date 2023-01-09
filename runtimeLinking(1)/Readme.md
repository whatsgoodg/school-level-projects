# 기능
(경도, 위도) (경도, 위도) 형태의 데이터가 저장된 텍스트파일에서 데이터를 받아와 두 가지의 shared library(libeuc.c, libman.c)를 통해 맨하튼 거리와 유클리드 거리를 출력하는 API의 형태로과제를 구현했습니다.              
소스코드 파일은 총 5개입니다. coordi.h 헤더파일에 두 개의 구조체와 getGIS, setInfos, print_featrues 함수가 있습니다. coordi.c는 세 개의 함수 정의부입니다.                     
man.c, euc.c 파일은 각각 맨하튼과 유클리드 거리를 구하여 출력하는 함수입니다.                     
main.c 파일은 텍스트 파일과 동적라이브러를 받아와 total second를 계산하고 저장하여 거리를 계산합니다.                  
# 코드 설명
 ![image](https://user-images.githubusercontent.com/86244920/211327883-3959f167-6402-4454-864d-848e77ad5c4e.png)                
                   
main.c 파일은 위와 같습니다. 커맨드 라인에 API를 실행할 때, 텍스트 파일명과 동적 라이브러리 파일명은 전달하여 argv[] 배열에 전달합니다.                 
이후 dlopen 함수를 호출하여 인자로 전달된 동적라이브러리의 handle을 반환받습니다.                   
이후 라이브러리가 존재하지 않거나 로딩에 실패했다면 NULL을 반환하고 dlopen의 에러 사유가 dlerror 함수를 통해 출력됩니다.	           
그리고 dlsym 함수를 통해 인자로 handle과 함수명을 전달한다. 함수가 존재한다면 반환값으로 함수 주소값을 받습니다. 이를 함수 포인터로 저장합니다. NULL이 반환된다면 dlerror로 dlsym의 에러 사유를 출력합니다.                 
dlclose를 통해 동적 라이브러를 close합니다. 이후 0보다 크다면 error이기 때문에 dlerror로 에러 사유를 출력합니다. dlerror가 호출되는 모든 상황엔 프로그램이 종료됩니다.             
                 
![image](https://user-images.githubusercontent.com/86244920/211328224-79b7df73-72c2-4fc9-a543-2648873d20fe.png)              
              
x,y의 capacity는 1000으로 설정합니다. getGIS 함수를 통해 argv[]로 받은 input.txt에 있는 값을 구조체에 저장합니다. 이후 setInfos 함수를 통해 totalSec를 구합니다.            
                 
![image](https://user-images.githubusercontent.com/86244920/211328296-11a0aa82-4581-462e-be94-a65c8d0e348b.png)                  
                    
이후에 degree, minute, second, direction을 print_featrues 함수로 호출하고 print_distance 함수로 거리를 계산하고 출력합니다              
                                       
# 사용법
![image](https://user-images.githubusercontent.com/86244920/211328414-75a25b68-6c8f-4ade-bc1f-f6cda5a0f9fc.png)                 
                   
현재 디렉토리에 존재하는 파일입니다.(temp.c는 백업파일)              
분할 컴파일을 통해 main.c과 coordi.c를 컴파일합니다.                
                 
 ![image](https://user-images.githubusercontent.com/86244920/211328491-adbb55fd-ec9c-44b3-a3ab-4bb0a39b1986.png)             
                
이후에 man.c 파일을 shared library로 컴파일합니다.               
               
![image](https://user-images.githubusercontent.com/86244920/211328556-778acc63-c3f9-4c7a-a343-993458046e05.png)                
                    
euc.c 파일은 -lm옵션을 주어야합니다. math.h 헤더의 sqrt 함수를 사용하고 있기 때문에 링커에게 알려줘야합니다. -lm은 libm을 뜻합니다.          
                
![image](https://user-images.githubusercontent.com/86244920/211328625-87a7d010-3089-45eb-9021-3eeed3ba4390.png)                

컴파일 이후 파일 목록입니다.
 
# 출력값.
API 구현을 통해 동적으로 라이브러리가 추가되어 함수가 호출되는 부분을 잘 이해한 것 같습니다.             
윈도우 11 업데이트 이후 버츄어 박스가 제대로 작동하지 않아. 우분투로 과제를 수행하고 깃허브 push를 통해 소스코드와 실행파일을 다운받았습니다.                
