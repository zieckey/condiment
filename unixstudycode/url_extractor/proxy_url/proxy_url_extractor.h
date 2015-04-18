
#ifndef PROXY_URL_EXTRACTOR_H_
#define PROXY_URL_EXTRACTOR_H_

#include <string>
#include <set>
#include <set>

namespace url
{

    /**
     * \brief ����url��ȡ��
     *  ��Ե�ǰ������������ַ��������302��תurl������url��©�����ƹ���ͳɱ�����
     * ����ο���http://3g.hebei.com.cn/system/2014/01/06/013162824.shtml
     */
    class ProxyURLExtractor
    {
    public:
        typedef std::set<std::string/*proxy key*/> KeyItems;

    public:
        ProxyURLExtractor();

        //! \brief ���ع����ļ�
        //! \param[in] - const std::string & rule_file
        //! \return - bool
        bool Initialize(const std::string& rule_file);

        //! \brief ������ȡ������url�������ȡʧ�ܣ����ؿմ�
        //! \param[in] - const std::string & url
        //! \return - std::string
        std::string Extract(const std::string& url);

        static void Extract(const KeyItems& keys, const std::string& raw_url, std::string& sub_url);
        static std::string Extract(const KeyItems& keys, const std::string& raw_url);

    private:

        KeyItems keys_set_;
    };
}

#endif //PROXY_URL_EXTRACTOR_H_

