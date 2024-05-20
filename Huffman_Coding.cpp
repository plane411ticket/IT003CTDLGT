#include <bits/stdc++.h>
using namespace std;

// Map lưu tần suất các ký tự xuất hiện trong chuỗi kí tự đầu vào
map<char, int> tansuat;

// Map lưu mã Huffman của từng ký tự xuất hiện trong chuỗi kí tự đầu vào
map<char, string> luukq;

// String lưu dãy kết quả đã mã hóa
string dayketquamahoa;


struct Node
{
    // Chứa từng ký tự và tần suất của nó

    char data; // Ký tự
    int freq; // Tần suất

    // Con trỏ đến Node bên phải và bên trái
    struct Node *left, *right;
};

struct Tree
{
    //Chứa các Node

    int kichthuoc; // Kích thước của cây hiện tại

    int succhuamax; // Sức chứa tối đa của cây

    struct Node** mang; //Một mảng con trỏ trỏ vào các Node của cây
};

struct Node* newNode(char dulieu, int tan_suat)
{
    // Hàm tạo node mới từ data và freq được cung cấp

    struct Node* luu = (struct Node*)malloc(sizeof(struct Node)); // Cấp phát vùng nhớ mới cho node

    luu->data = dulieu;
    luu->freq = tan_suat;

    luu->left = NULL;
    luu->right = NULL;

    return luu;
}

void Heapify(struct Tree* huffmantree, int j)
{
    // Hàm để duy trì tính chất của min heap: nút nhỏ nhất ở gốc, giúp việc lấy ra 2 phần tử nhỏ nhất dễ dàng
    // Nếu nút trái/phải < nút cần xét, hoán đổi vị trí

    int nho = j;
    int left = 2 * nho;
    int right = 2 * nho + 1;
    if ((left < huffmantree->kichthuoc) && (huffmantree->mang[left]->freq < huffmantree->mang[nho]->freq))
    {
        nho = left;
    }
    if ((right < huffmantree->kichthuoc) && (huffmantree->mang[right]->freq < huffmantree->mang[nho]->freq)) {
        nho = right;
    }
    if (nho != j)
    {
        struct Node* luu = huffmantree->mang[nho];
        huffmantree->mang[nho] = huffmantree->mang[j];
        huffmantree->mang[j] = luu;
        Heapify(huffmantree, nho); // Gọi lại hàm Heapify để đảm bảo tính chất của heap
    }
}

struct Node* PhanTuNhoNhat(struct Tree* huffmantree)
{
    // Hàm lấy phần tử nhỏ nhất từ heap

    struct Node* luu = huffmantree->mang[0];

    huffmantree->mang[0] = huffmantree->mang[huffmantree->kichthuoc - 1];

    huffmantree->kichthuoc--;//Lấy ra 1 phần tử, nên kích thước giảm đi 1

    Heapify(huffmantree, 0);

    return luu;
}

void ChenThemPhanTu(struct Tree* huffmantree, struct Node* top)
{
    // Hàm chèn thêm phần tử vào heap

    huffmantree->kichthuoc++; //Chèn thêm phần tử nên kích thước tăng lên 1
    int k = huffmantree->kichthuoc - 1;

    while (k && top->freq < huffmantree->mang[(k - 1) / 2]->freq)
    {
        huffmantree->mang[k] = huffmantree->mang[(k - 1) / 2];
        k = k / 2;
    }
    huffmantree->mang[k] = top;
}

struct Node* BuildHuffmanTree()
{
    // Hàm xây dựng cây Huffman

    struct Node *top, *right, *left;

        //Khởi tạo cây

    struct Tree* huffmantree = (struct Tree*)malloc(sizeof(struct Tree)); // Cấp phát vùng nhớ mới cho cây
    huffmantree->kichthuoc = 0;
    huffmantree->succhuamax = tansuat.size();
    huffmantree->mang = (struct Node**)malloc(huffmantree->succhuamax * sizeof(struct Node*)); //Cấp phát bộ nhớ cho mảng có kích thước là số phần tử max của mảng

    int chiso = 0;
    for (auto it = tansuat.begin(); it != tansuat.end(); ++it)
    {
        // Nạp dữ liệu cho các node trong cây

        huffmantree->mang[chiso] = newNode(it->first, it->second);
        chiso++;
    }
    huffmantree->kichthuoc = tansuat.size();

    for (int j = (huffmantree->kichthuoc - 1) / 2; j >= 0; j--)
    {
        //Đảm bảo tính Min Heap của cây khởi tạo ở trên

        Heapify(huffmantree, j);
    }

    while (huffmantree->kichthuoc != 1)
    {
        //Lấy ra hai phần tử nhỏ nhất trong cây

        left = PhanTuNhoNhat(huffmantree);
        right = PhanTuNhoNhat(huffmantree);

        //Tạo Node mới có freq bằng tổng 2 nút nhỏ nhất trong cây hiện tại

        top = newNode('$%', left->freq + right->freq);
        top->left = left;
        top->right = right;

        //Chèn thêm phàn tử mới khởi tạo vào cây

        ChenThemPhanTu(huffmantree, top);
    }
    return PhanTuNhoNhat(huffmantree);
}

void InTungKyTu(int arr[], int n)
{
    // Hàm in mảng

    for (int i = 0; i < n; ++i)
    {
        cout << arr[i];
    }
    cout << "\n";
}

int La(struct Node* root)
{
    // Hàm kiểm tra nút lá: nếu bên trái và phải không còn nút khác sẽ là nút lá

    return !(root->left) && !(root->right);
}

void KyTuMaHoa(struct Node* root, int arr[], int top)
{
    // Hàm in mã Huffman của từng ký tự

    if (root->left)
    {
        arr[top] = 0;
        KyTuMaHoa(root->left, arr, top + 1);
    }

    if (root->right)
    {
        arr[top] = 1;
        KyTuMaHoa(root->right, arr, top + 1);
    }

    if (La(root))
    {
        // Nếu là nút lá thì in giá trị mã hóa của từng ký tự
        cout << root->data << ": ";
        InTungKyTu(arr, top);

        // Lưu lại kết quả mã hóa của từng ký tự
        string code = "";
        for (int i = 0; i < top; i++)
        {
            code += to_string(arr[i]);
        }
        luukq[root->data] = code;
    }
}

void InCaDay(string xau)
{
    // Hàm in ra dãy mã hóa của chuỗi đầu vào

    for (int i = 0; i < xau.size(); i++)
    {
        cout << luukq[xau[i]];
        dayketquamahoa += luukq[xau[i]];
    }

    cout << "\n\n";
    float a= xau.size() * 8;
    float b= dayketquamahoa.size();
    cout << "\nTotal ASCII code: " << a << "\n\n";
    cout << "Total Huffman Code of string: " << b << "\n\n";
    cout << "Performance of Huffman Code: "  << ((a - b)/a)*100 << "\n\n";
}

void Decoding(struct Node* root, string xaumh)
{
    // Hàm giải mã chuỗi đã mã hóa
    struct Node* giatri = root;
    int tong = 0;
    for (int i = 0; i < xaumh.size(); i++)
    {
        if (xaumh[i] == '0')
        {
            giatri = giatri->left;
        }
        else
        {
            giatri = giatri->right;
        }
        if (La(giatri))
        {
            // Nếu là nút lá thì in ra Data ở nút đó
            // Đưa cây về lại nút gốc để giải mã ký tự tiếp theo
            cout << giatri->data;
            giatri = root;
            tong += 1;
        }
    }
    cout << "\nSize of string after decoding: "<< tong;
}

void HuffmanCoding(string xau)
{
    // Hàm thực hiện mã hóa Huffman

    struct Node* root = BuildHuffmanTree(); //Xây dựng cây Huffman
    int arr[100000], top = 0;
    cout << "=====================================================";
    cout << "\nPrint Huffman Code of each letter:\n";
    KyTuMaHoa(root, arr, top);
    cout << "\nEncoding the input string with Huffman Code above:\n";
    InCaDay(xau);
    cout << "=====================================================";
    cout << "\nDecoding the encoded string:\n";
    Decoding(root, dayketquamahoa);
    cout << "\n";
}

int main()
{
    string xau;
    getline(cin, xau);
    for (int i = 0; i < xau.size(); i++)
    {
        tansuat[xau[i]]++;
    }
    for (auto it = tansuat.begin(); it != tansuat.end(); ++it)
    {
        cout << it -> first <<" " << it -> second << "\n";
    }
    if (tansuat.size() == 1)
    {
        cout << "\nTotal ASCII code: " << xau.size() * 8 << "\n\n";
        cout << "Print Huffman Code of each letter: " << xau[0] << ": 0\n\n";
        cout << "Print Huffman Code of input string:\n";
        for (int i = 0; i < xau.size(); i++)
        {
            cout << 0;
        }
        cout << "\n\n";
        cout << "Total Huffman Code of string: " << xau.size();
    }
    else
    {
        HuffmanCoding(xau);
    }
    return 0;
}
