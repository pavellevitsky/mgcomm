/*!
 * @file ModemAt.hh
 *
 * @brief Modem AT Commands I/O
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * This class is an example of AT Commands communication with modem
 */

class ModemAt
{
    public:

        /*! Connect to modem AT channel
         *
         * @param    device_name - Unix device file name
         * @param    debug - Enable debugging (dumps)
         */
        ModemAt(const char *device_name = "/dev/smd11", bool debug = false);

        ~ModemAt();

        /*! Execute AT Command
         * @param    command - AT Command (termination?)
         * @param    timeout - Timeout in seconds
         *
         * @return    Response of AT Command
         */
        std::string at_cmd(std::string command, int timeout = 4);

    private:

        int fd_;    //! file descriptor
        bool debug_;

        /*! Read from modem channel
         *
         * \param    timeout        timeout in s (0 - no wait)
         *
         * \return    Whatever been read from modem within the given time
         */
        std::string read_(unsigned timeout = 4);

        /*! Write to modem channel
         * \param    command    AT Command without prefix and termination
         */
        void write_(const std::string command);
};    // class ModemAt
