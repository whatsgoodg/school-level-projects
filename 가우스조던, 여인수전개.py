###########################################################################
# HEADER COMMENTS (5 pts)
###########################################################################
### __filename__ = "cse2105_ps003.py"
### __Python version__ = "3.10"
### __author__ = "Kijeong Kim"
### __copyright__ = "Copyright 2022, CSE2015 Problem Set 03"
### __credits__ = ["Jieung Kim", "Kijeong Kim"]
### __license__ = "GPL"
### __version__ = "1.0.0"
### __maintainer__ = "Kijeong Kim"
### __email__ = "12213426@inha.ac.kr"
###########################################################################

from typing import Optional
import copy 
import random
import time
import sys

###########################################################################
# determinant_with_cofactor (25 pts)
###########################################################################
def getMinorMatrix(matrix, i, j): # 소행렬을 구하는 함수 matrix는 기존 행렬 여인수를 위한 소행렬 기준 행,열 i,j
    n = len(matrix) 
    tmp = [[0 for col in range(n-1)] for row in range(n-1)] # n-1xn-1 소행렬 생성
    for a in range(0, n-1): # 소행렬 원소 초기화
        k = a if (a < i) else a+1 # a가 i보다 크거나 같다면 기존 행렬의 i를 배제하기에 +1
        for b in range(0, n-1):
            l = b if (b < j) else b+1 # 행과 같은 이유
            tmp[a][b] = matrix[k][l] # 삽입
    return tmp #소행렬 반환

def determinant_with_cofactor(matrix: list[list[int]]) -> Optional[int]:
  """ 
  여인수 전개를 위해 n-1xn-1 행렬을 구해주는 getMinorMatrix 함수를 사용하였습니다.
  그리고 이 함수는 base condtion이 2x2 행렬인 재귀함수로 구현하였습니다.
  모든 소행렬이 2x2까지 분해되었을 때 base condition에 충족되므로 재귀함수의 반복문이 끝이나 함수가 종료됩니다.  
  - Add your explanations about this function
  
  :param matrix: cofactor expansion을 사용하여 행렬식을 구할 행렬이 들어간다. 

  :return: 크기가 1보다 작거나, 정사각행렬이 아니면 None 리턴
   2x2 행렬일 경우(base condtion) ad - bc 리턴 
   base condtion 이후 행렬값 리턴.
  (more details about the return value)
  """
  if(len(matrix[0]) != len(matrix) or len(matrix) < 1): # 행렬의 크기가 1보다 작거나 정사각행렬이 아닐 경우에 None 반환
    return None

  if len(matrix) == 2: # 2x2 소행렬까지 분해됐으면 행렬식 반환
    return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0])
    
  detVal = 0 # 여인수 전개를 전부 더할 변수
  for c in range(len(matrix)):
    detVal += ((-1)**c)*matrix[0][c]*determinant_with_cofactor(getMinorMatrix(matrix,0,c)) # 소행렬 식을 구하여 재귀함수로 수를 누적.

  return detVal # 누적된 행렬식 반환
  pass
###########################################################################
# determinant_with_gauss (25 pts)
###########################################################################
def sub(matrix, i): # 주대각 성분이 0이라면 그 행 밑의 행들과 덧셈
  for t in range(i + 1, len(matrix)):
    for k in range(len(matrix)): 
      matrix[i][k] += matrix[t][k]

def sub2(matrix, i): # 시작전 가우스 조던 행 덧셈 함수
  for t in range(len(matrix)):
    if(t != i):
      for k in range(len(matrix)): 
        matrix[i][k] += matrix[t][k]

def determinant_with_gauss(matrix: list[list[int]]) -> Optional[int]:
  """ 
  가우스 조던 소거법 중 행렬식을 보존하는 특정 행에 스칼라배 이후 다른 행에 더하는
  행 연산을 사용하여 상삼각행렬을 만들고 주대각성분을 모두 곱하여 반환하는 함수입니다. 
  sub과 sub2 함수를 사용하여 주대각 성분이 0일 때 행 연산 중 덧셈을 하여 정확한 계산을 기대합니다. - Add your explanations about this function
  
  :param matrix: 가우스 조던 제거를 행할 행렬

  :return: 행렬식값 (more details about the return value)
  """  
  n = len(matrix) # 행의 크기
  m = len(matrix[0]) # 열의 크기 (행의 크기가 최소 1이기에 0 번째 인덱스의 길이)
  if(n != m or n < 1): return None # 정사각 행렬이 아닐 경우
  if(n == 1): return matrix[0][0]
  tmp = [0 for col in range(n)] # 스칼라배를 저장하기 위한 리스트
  for i in range(n): # 제거 시작전 주대각 성분이 0이라면 행 덧셈.
    if matrix[i][i] == 0: sub2(matrix, i)

  for i in range(0, n): # 상삼각 행렬을 만들기 위한 for문
      if matrix[i][i] == 0: # 주대각 성분이 하나라도 0일경우 행렬식은 0
          print(matrix)
          return 0
      piv = matrix[i][i] # 주대각 성분.
      if piv == 0: # 주대각 성분이 하나라도 0일경우 밑의 행렬들과의 덧셈
        sub(matrix, i)
        piv = matrix[i][i] 
        if piv == 0: return 0 # 그래도 주대각 성분이 0일 경우 비가역행렬
      for k in range(i, m): 
        tmp[k] = matrix[i][k] / piv # 처리되는 행에서의 스칼라배를 저장( matrix[0:n,i]] - matrix[0:n,i]] * matrix[i, 0:m] / matrix[i, i] ) 중 matrix[i, 0:m] / matrix[i, i]
        
      for k in range(0, n): # 상삼각 행렬을 만들어주기 위한 주대각 성분이 있는 열의 아래 행들을 모두 0으로 만들어주는 반복문
          if k > i: 
              s = matrix[k][i]  # 최종 스칼라배 ( matrix[0:n,i]] - matrix[0:n,i]] * matrix[i, 0:m] / matrix[i, i] ) 중 matrix[0:n, i]의 matrix[k, i] 부분 0이 될 자리

              for j in range(i,m): # 주대각 성분을 처리하는 행에 스칼라배 이후 밑의 행들을 더해주는 반복문
                  matrix[k][j] = (matrix[k][j] - (s * tmp[j])) 
  determinant = 1 # 가역행렬일 경우
  for i in range(len(matrix)): # 주대각성분의 곱
      determinant = determinant * matrix[i][i] 
  return round(determinant) 

  pass

##########################################################################
# determinant_with_cofactor_benchmarks (12.5 pts)
##########################################################################

# 추가된 부분만 주석을 달았습니다.
def getMinorMatrix_for_benchmarks(matrix, i, j): # 추가된 부분x 소행렬식엔 덧셈과 곱셈이 없음.
  n = len(matrix)
  tmp = [[0 for col in range(n-1)] for row in range(n-1)]
  for a in range(0, n-1):
      k = a if (a < i) else a+1
      for b in range(0, n-1):
          l = b if (b < j) else b+1
          tmp[a][b] = matrix[k][l]
  return tmp
def determinant_with_cofactor_benchmarks(matrix: list[list[int]]) -> tuple[Optional[int], int, int]: # 덧셈 곱셈 순서
  """ 
  기존의 여인수 전개에서 2x2 행렬의 행렬식, 여인수 부분의 (-1)^(i+j) 부분을 제외하고 덧셈과 곱셈을 카운트하는 함수.
   - Add your explanations about this function
  
  :param matrix: 기존과 같음.

  :return: 재귀함수의 특성 때문에 덧셈, 곱셈 순서대로 카운트 반환. (more details about the return value)
  """
  add_cnt = 0
  mul_cnt = 0
  if(len(matrix) == 1):
    return matrix[0][0], 0, 0

  if(len(matrix[0]) != len(matrix)):
    return None, 0, 0

  if len(matrix) == 2:
    mul_cnt += 2 #추가된 부분 2x2행렬의 행렬식은 1번의 뎃섬과 2번의 곱셈
    add_cnt += 1
    return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]), add_cnt, mul_cnt
    
  detVal = 0
  for c in range(len(matrix)):
    tmp = (determinant_with_cofactor_benchmarks(getMinorMatrix_for_benchmarks(matrix,0,c)))  
    detVal += ((-1)**c)*matrix[0][c]*tmp[0]
    add_cnt += tmp[1] + 1 # 추가된 부분 누적된 cnt 변수들도 튜플 형태로 반환하여 값 저장
    mul_cnt += tmp[2] + 2 # -1부분을 제외하고 두 번의 곱셈과 누적합을 위한 한 번의 덧셈
  return detVal, add_cnt, mul_cnt #누적을 위한 튜플 반환
  pass

##########################################################################
# determinant_with_gauss_benchmarks (12.5 pts)
##########################################################################
def sub_bench(matrix, i): 
  add_cnt = 0 # 덧셈 부분 카운트
  for t in range(i + 1, len(matrix)):
    for k in range(len(matrix)): 
      matrix[i][k] += matrix[t][k]
      add_cnt += 1
  return add_cnt # 덧셈 값 반환

def sub_bench2(matrix, i): # 덧셈 부분 카운트
  add_cnt = 0
  for t in range(len(matrix)):
    if(t != i):
      for k in range(len(matrix)): 
        matrix[i][k] += matrix[t][k]
        add_cnt += 1 # 카운트
  return add_cnt # 반환

def determinant_with_gauss_benchmarks(matrix: list[list[int]]) -> tuple[Optional[int], int, int]: # 덧셈 곱셈 순서 반환
  """ 
  가우스-조던 소거법의 행 연산 중, 스칼라배와 행 덧셈 그리고 주대각성분의 곱셈을 카운팅하는 함수 
  - Add your explanations about this function
  
  :param matrix: 기존과 같음

  :return: 카운트한 변수 두 개 추가 덧셈과 곱셈 순서로 반환. (more details about the return value)
  """   
  n = len(matrix) 
  m = len(matrix[0]) 
  if(n != m or n < 1): return None
  if(n == 1): return matrix[0][0], 0, 0 # 덧셈과 뺄셈 번환 추가

  add_cnt = 0 # 덧셈 카운트
  mul_cnt = 0 # 곱셈 카운트
  for i in range(n): 
    if matrix[i][i] == 0: add_cnt += sub_bench2(matrix, i)

  tmp = [0 for col in range(n)] 
  for i in range(0, n): 
      piv = matrix[i][i] 
      if piv == 0: 
        add_cnt = sub_bench(matrix, i)
        piv = matrix[i][i]
        if piv == 0: return 0, add_cnt, mul_cnt
      for k in range(i, m): 
        tmp[k] = matrix[i][k] / piv 
        mul_cnt += 1 # 나누기 => 곱셈
        
      for k in range(0, n): 
        if k > i: 
          s = matrix[k][i]  
          for j in range(i,m): 
            matrix[k][j] = (matrix[k][j] - (s * tmp[j])) 
            mul_cnt += 1 # 위의 연산의 덧셈 곱셈 개수
            add_cnt += 1
  determinant = 1
  for i in range(len(matrix)):
      determinant = determinant * matrix[i][i] 
      mul_cnt += 1 # 곱셈 카운트 주대각 성분의 곱 
  return round(determinant), add_cnt, mul_cnt # 반환 값 추가
  pass
 
##########################################################################
# run_benchmarks (15 pts)
##########################################################################
def run_benchmarks():
  """ 
  함수 전후로의 time.time() 함수를 받아 시간을 측정하고
  각각 알고리즘의 덧셈, 곱셈 연산 횟수를 반환받고 출력하는 함수.
  seed를 0으로 해놓아 한 번 발생한 난수로 고정시켰습니다.
  randint를 사용하여 0부터 7 사이의 정수를 랜덤으로 반환받았습니다. - Add your explanations about this function
  # Matrix size: 3 x 3
#   Using Cofactor expansion: 
#     Average time (sec): 0.0009999275207519531
#     Average num. of additions: 6
#     Average num. of multiplications: 12
#   Using Gauss elimination: 
#     Average time (sec): 0.0
#     Average num. of additions: 8
#     Average num. of multiplications: 17 
#
  """
  random.seed(0)
  for i in range(1, 12):
    print("------------------------------------------")
    ave_time_c, ave_add_c, ave_mul_c = 0,0,0 # 평균 시간, 덧셈 평균, 곱셈 평균
    ave_time_g, ave_add_g, ave_mul_g = 0,0,0
    flag = True
    for j in range(10):
      matrix = [[random.randint(0, 7) for col in range(i)] for row in range(i)] # [0, 7] 랜덤 난수 발생
      matrix2 = copy.deepcopy(matrix) # 깊은 복사
      det1, det2 = 0,0 # 행렬값 체크
      start = time.time() # 여인수 시작 시간
      tmp = determinant_with_cofactor_benchmarks(matrix) 
      det1 = tmp[0]
      ave_add_c += tmp[1] # 덧셈 곱셈 누적
      ave_mul_c += tmp[2]
      ave_time_c += time.time() - start # 끝
      start = time.time() # 가우스 시작 시간
      tmp = determinant_with_gauss_benchmarks(matrix)
      ave_time_g += time.time() - start # 끝
      det2 = tmp[0]
      ave_add_g += tmp[1]
      ave_mul_g += tmp[2]
      if(det1 != det2): flag = False # 행렬식 값이 다르면 False
    print("Matrix size:", i, "x", i)
    print("Using Cofactor expansion:")
    print("   Average time (sec):", ave_time_c / 10) # 시간 평균
    print("   Average num. of additions:", ave_add_c / 10 ) # 덧셈 평균
    print("   Average num. of multiplications:", ave_mul_c / 10) # 곱셈 평균
    print("Using Gauss elimination:")
    print("   Average time (sec):", ave_time_g / 10)
    print("   Average num. of additions:", ave_add_g / 10 )
    print("   Average num. of multiplications:", ave_mul_g / 10)
    if(flag == True): print("Correct") # 값이 정확한지 확인
    else: print("Wrong answer")


    

    

    


  pass

##########################################################################
# main functions. Please do not change the following two functions
##########################################################################
def main() -> int:
  """ 
  Main function that runs run_benchmarks function in it. 
  """
  
  run_benchmarks()
  return 0

if __name__ == '__main__':
  sys.exit(main())

###########################################################################
# TABLE (5 pts)
###########################################################################
# Matrix size: 3 x 3
#   Using Cofactor expansion: 
#     Average time (sec): 0.0
#     Average num. of additions: 6
#     Average num. of multiplications: 12
#   Using Gauss elimination: 
#     Average time (sec): 0.0
#     Average num. of additions: 9.8
#     Average num. of multiplications: 17 
#
# Matrix size: 4 x 4
#   Using Cofactor expansion:
#     Average time (sec): 9.996891021728516e-05
#     Average num. of additions: 28
#     Average num. of multiplications: 56
#   Using Gauss elimination: 
#     Average time (sec): 0.0
#     Average num. of additions: 26
#     Average num. of multiplications: 34
#
# Matrix size: 5 x 5
#   Using Cofactor expansion:
#     Average time (sec): 0.0007999658584594727
#     Average num. of additions: 145
#     Average num. of multiplications: 290 
#   Using Gauss elimination: 
#     Average time (sec): 0.00010008811950683594
#     Average num. of additions: 51.5
#     Average num. of multiplications: 60.0
#
# Matrix size: 6 x 6
#   Using Cofactor expansion:
#     Average time (sec): 0.005165362358093261
#     Average num. of additions: 876
#     Average num. of multiplications: 1752 
#   Using Gauss elimination: 
#     Average time (sec): 9.677410125732421e-05
#     Average num. of additions: 84.4
#     Average num. of multiplications: 97 
#
# Matrix size: 7 x 7
#   Using Cofactor expansion:
#     Average time (sec): 0.03699491024017334
#     Average num. of additions: 6139
#     Average num. of multiplications: 12278 
#   Using Gauss elimination: 
#     Average time (sec): 0.00010125637054443359
#     Average num. of additions: 158.2
#     Average num. of multiplications: 147 
#
# Matrix size: 8 x 8
#   Using Cofactor expansion:
#     Average time (sec): 0.3066966772079468
#     Average num. of additions: 49120
#     Average num. of multiplications: 98240 
#   Using Gauss elimination: 
#     Average time (sec): 0.00010716915130615234
#     Average num. of additions: 218.4
#     Average num. of multiplications: 212 
#
# Matrix size: 9 x 9
#   Using Cofactor expansion:
#     Average time (sec): 3.3740094184875487
#     Average num. of additions: 442089
#     Average num. of multiplications: 884178
#   Using Gauss elimination: 
#     Average time (sec): 0.0010552406311035156
#     Average num. of additions: 303.7
#     Average num. of multiplications: 294 
#
# Matrix size: 10 x 10
#   Using Cofactor expansion:
#     Average time (sec): 29.501713967323305
#     Average num. of additions: 4420900
#     Average num. of multiplications: 8841800
#   Using Gauss elimination: 
#     Average time (sec): 0.0010025501251220703
#     Average num. of additions: 428
#     Average num. of multiplications: 395
#
# Matrix size: 11 x 11
#   Using Cofactor expansion:
#     Average time (sec): 302.33460235595703
#     Average num. of additions: 48629911 
#     Average num. of multiplications: 97259822
#   Using Gauss elimination: 
#     Average time (sec): 0.002001523971557617
#     Average num. of additions: 440
#     Average num. of multiplications: 517 
#
###########################################################################
