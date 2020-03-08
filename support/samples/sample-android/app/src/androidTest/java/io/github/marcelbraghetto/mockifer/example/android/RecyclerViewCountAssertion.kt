package io.github.marcelbraghetto.mockifer.example.android

import androidx.test.espresso.NoMatchingViewException
import androidx.test.espresso.ViewAssertion
import androidx.recyclerview.widget.RecyclerView
import android.view.View
import org.hamcrest.CoreMatchers.`is`
import org.hamcrest.Matcher
import org.hamcrest.MatcherAssert.assertThat
import java.lang.IllegalArgumentException

internal class RecyclerViewCountAssertion private constructor(private val matcher: Matcher<Int>) : ViewAssertion {

    override fun check(view: View, noViewFoundException: NoMatchingViewException?) {
        if (noViewFoundException != null) {
            throw noViewFoundException
        }

        val recyclerView = view as RecyclerView
        val adapter = recyclerView.adapter
        val itemCount = adapter?.itemCount ?: throw IllegalArgumentException("No adapter items found")
        assertThat(itemCount, matcher)
    }

    companion object {

        fun withItemCount(expectedCount: Int): RecyclerViewCountAssertion {
            return withItemCount(`is`(expectedCount))
        }

        fun withItemCount(matcher: Matcher<Int>): RecyclerViewCountAssertion {
            return RecyclerViewCountAssertion(matcher)
        }
    }
}

