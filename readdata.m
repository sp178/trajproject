%读取二进制数据
f=fopen('test.spdata','r');
 [data,count]=fread(f,4,'uint64');%读取头
 length = data(4)/(data(1)*data(3));
 table = zeros(length,data(3));
 for i=1:length
     table(i,:)=fread(f,data(3),'double')';
 end