# 1. sleep
## Giải pháp
- Chương trình kiểm tra số lượng đối số đầu vào thông qua biến `argc`. Nếu người dùng không nhập đúng 1 tham số (`argc == 2`), chương trình sẽ báo lỗi và exit.
- Kiểm tra input của người dùng có phải là số nguyên dương không bằng cách duyệt qua từng kí tự của input, nếu phát hiện kí tự lạ thì báo lỗi và exit.
- Sử dụng hàm `atoi()` để chuyển đổi tham số từ dạng string sao số nguyên và lưu vào biến `ticks`.
- Thực hiện system call `sleep()` và truyền tham số là biến `ticks` vào để tạm dừng tiến trình.
- exit sau khi hoàn thành.


# 2. pingpong
## Giải pháp
- Sử dụng 2 pipe để thiết lập giao tiếp 2 chiều: `fd1` được dùng để cha gửi dữ liệu sang con và `fd2` được dùng để con gửi lại dữ liệu sang cha.
- Dùng hàm `fork()` để tạo tiến trình con.
	- Tiến trình con sẽ đợi đọc 1 byte từ `fd1[0]`, nếu đọc được thì thông báo, sau đó gửi lại 1 byte cho cha thông qua `fd2[1]`.
	- Tiến trình cha sẽ gửi 1 byte cho con thông qua `fd1[1]`, sau đó đợi 1 byte từ con gửi về lại thông qua `fd2[0]`, nếu đọc được thì thông báo. Sau đó đợi tiến trình con đóng rồi mới tiếp tục thực thi.
- exit sau khi hoàn thành ở cả 2 tiến trình cha và con.
## Giải pháp khác
- Thử dùng 1 pipe duy nhất `fd` cho cả 2 chiều giao tiếp để tiết kiệm tài nguyên
- Dùng hàm `fork()` để tạo tiến trình con.
	- Tiến trình cha gửi 1 byte cho con thông qua `fd[1]`, sau đó đợi 1 giây và đọc dữ liệu từ `fd[0]`.
	- Trong 1 giây đó, tiến trình con đọc 1 byte từ `fd[0]`, sau đó gửi 1 byte cho cha thông qua `fd[1]`. 
	- Sau khi hết 1 giây lúc này dữ liệu `fd[0]` là dữ liệu được gửi từ tiến trình con, nên tiến trình cha có thể đọc.
###### Đánh giá: Không ổn định, có nhiều rủi ro 
- Không xác định được thời gian cụ thể để sleep chương trình.
- Race condition: Nếu không có cơ chế khóa (locking) hoặc đồng bộ hóa chặt chẽ, việc hai tiến trình cùng tranh giành một đầu đọc/ghi của một pipe duy nhất sẽ có khả năng cao dẫn đến việc dữ liệu bị đọc nhầm. -> tiến trình cha có thể đọc lại dữ liệu do chính mình gửi đi.

# 3. primes
# Giải pháp
- Sử dụng mô hình Concurrent Pipeline dựa trên ý tưởng của Doug McIlroy, với mỗi tiến trình con, ta sẽ tìm ra một số nguyên tố `prime` và sàng lọc bớt các số nguyên chia hết cho số `prime` đó.
- Tiến trình gốc khởi tạo một pipe ban đầu, đẩy dãy số nguyên từ 2 đến 280 vào pipe.
- Dùng hàm `fork()` để tạo tiến trình con.
	- Tại tiến trình con, lấy số đầu tiên từ pipe ra, đó chính là `prime`, nếu không lấy được thì có nghĩa là trong pipe không còn số nào nữa -> kết thúc tiến trình.
	- In số `prime` này ra màn hình.
	- Tạo pipe mới và dùng hàm `fork()` để tạo tiếp tiến trình con, nhiệm vụ của tiến trình con cũng giống như tiến trình cha (đợi đọc các số nguyên từ trong pipe do tiến trình cha gửi tới, sàng lọc bội số của số nguyên tố và đẩy các số không phải bội sang pipe tiếp theo).
	- Sau đó lấy hết các số nguyên còn lại trong pipe và kiểm tra xem nó có là bội của số `prime` không, nếu không thì đẩy số đó vào pipe mới.
	- Sau khi đẩy xong hết thì đợi cho tiến trình con exit, rồi sau đó exit.

# 4. cp
## Giải pháp
- Chương trình thực hiện sao chép dữ liệu từ file nguồn (`src`) sang file đích (`dst`) dựa trên các tham số dòng lệnh.
- Kiểm tra số lượng đối số, nếu người nhập thiếu tham số `src` hoặc `dst`, chương trình sẽ báo lỗi và exit.
- Tạo một mảng `fd` (file descriptor) gồm 2 phần tử: `fd[0]` để đọc và `fd[1]` để ghi
- Thực hiện system call `open()` để mở file nguồn với chế độ chỉ đọc (`O_RDONLY`) sau  và file đích với các cờ (`O_WRONLY | O_CREATE`) để tạo mới (nếu file không tồn tại) và ghi đè.
- Trỏ `fd[0]` tới file `src` và `fd[1]` tới file `dst`
- Dữ liệu được truyền tải bằng cách sử dụng vòng lặp `while` và đọc theo từng buffer kích thước 512 bytes. Vòng lặp sẽ tiếp tục cho đến khi nào `read()` trả về `0` (hết file).
- Sau khi truyền tải hoàn tất thì exit.


# 5. tree
## Giải pháp
- Sử dụng hàm đệ quy `tree()` để duyệt qua cấu trúc cây thư mục, bắt đầu từ đường dẫn do người dùng chỉ định thông qua tham số dòng lệnh.
- Sử dụng một mảng `is_last` để xác định đã đi tới nhánh cuối cùng chưa tại mỗi cấp độ, từ đó in ra các ký tự nối nhánh (`├───` hoặc `└───`) một cách chính xác.
- Xử lý tùy chọn: Triển khai các biến toàn cục `DIRONLY` (cho `-d`) và `LEVEL` (cho `-L`) để điều hướng logic duyệt: bỏ qua tệp tin hoặc dừng đệ quy khi đạt độ sâu giới hạn.
- Sử dụng hệ thống gọi `fstat()` để phân biệt giữa tệp tin (`T_FILE`) và thư mục (`T_DIR`) nhằm thực hiện các hành động tương ứng.
- Quản lý bộ nhớ động: Sử dụng `malloc` và `free` để lưu trữ tạm thời danh sách tên các tệp tin trong thư mục, giúp xác định chính xác phần tử cuối cùng để in định dạng `└───`.
## Vấn đề gặp phải
- Đệ quy vô hạn: do mỗi thư mục điều chứa 2 thư mục con là `./` và `../`, nếu không kiểm tra và loại bỏ 2 thư mục này, chương trình có thể đệ quy từ thư mục hiện tại tới chính nó hoặc tới thư mục cha của nó, dẫn tới tình trạng đệ quy vô hạn.
# 6. du
## Giải pháp


# 7. diff
## Giải pháp 
