#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include <string>

int main()
{

    const char* errstr = NULL;
    int         erroff = 0;
    //pcre* regex = pcre_compile("axb", PCRE_CASELESS|PCRE_UTF8, &errstr, &erroff, NULL);
    pcre* regex = pcre_compile("axb", 0, &errstr, &erroff, NULL);
    
    std::string s1 = "xaxbLaxbLaxbLaxb";
    int ovector[10];
    int limit = 10;
    int rc = pcre_exec(regex, NULL, s1.data(), s1.size(), 0, 0, ovector, limit);
    printf("rc=%d\n", rc);
    int index = 0;
    while (rc > 0) {
        fprintf(stdout, "%s:%d ovector:", __FILE__, __LINE__);
        for (unsigned int li = 0; li < limit ; ++li ) {
            fprintf(stdout, "%d ", ovector[li]);
        }
        const char* result = NULL;
        if (pcre_get_substring(s1.data(), ovector, rc, 0, &result) >= 0) {
            size_t result_len = strlen(result);
            fprintf(stdout, "%s:%d rc=%d index=%d pcre_get_substring:[%s] len=%lu\n", __FILE__, __LINE__, rc, index, result, result_len);
            index = ovector[2*rc + 1];
        }
        rc = pcre_exec(regex, NULL, s1.data(), s1.size(), index, 0, ovector, limit);
    }

    return 0;
}

